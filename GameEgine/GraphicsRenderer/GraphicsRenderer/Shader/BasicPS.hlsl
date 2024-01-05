#include "Commons.hlsli"

// ����� �̹����� GPU �޸𸮿��� Texture2D �������� �޾ƿ´�.
Texture2D g_texture0 : register(t0); // t�� texture
// Environmental Light�� ����ϱ� ���� CubeMap Texture
TextureCube g_textureCube : register(t1);
TextureCube g_diffuseCube : register(t2);
TextureCube g_specularCube : register(t3);
// Texture Image ���� ���ϴ� ��ġ���� ������ �������� �۾��� Sampling�̶� �Ѵ�.
SamplerState g_sampler0 : register(s0); // s�� sampler

cbuffer MeshPSConstData : register(b0)
{
    float3 camWorld;            // ������ ���� ��ǥ�� ��ġ
    bool useTexture;    
    Material material;          // ����
    Light lights[MAX_LIGHTS]; // ����
    RimLight rimLights;
};

// object�� �׵θ��� ���� ���� ���� ���� ȿ��
float3 RimLighting(float3 color, float3 toCam, float3 normal)
{
    // ���� �߽ɿ� �������� dot product�� ���� 1.0�� ���������. (������ ������ rim���� 0.0�� ���������.)
    float rim = 1.0f - dot(toCam, normal);
    
    // Smoothstep
    // https://thebookofshaders.com/glossary/?search=smoothstep
    if (rimLights.useSmoothStep)
        rim = smoothstep(0.0f, 1.0f, rim); // �����Լ� ���
    
    rim = pow(abs(rim), rimLights.rimPower);

    return color + rim * rimLights.rimColor * rimLights.rimStrength;
}

// Environment Mapping (ȯ�� ����)
float4 IBLReflect(float3 toCam, float3 normal)
{
    // Reflect(������ ������ ����, ��� ����)
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-reflect
    // �̵��� ���ؼ��� �ݿ��� �ϸ� �ȵǱ⿡ normal�� ��������� view���� ������ �޴´�.
    // ��, �ݻ籤�� ����� �ֺ� ȯ�濡 ���� ���� ������. 
    //(�������� ��ü�� ǥ���� ���� �ݻ� ������ ���� �����´�)
    float4 cubeColor = g_textureCube.Sample(g_sampler0, reflect(-toCam, normal));
    return cubeColor;
}

// Schlick approximation: Eq. 9.17 in "Real-Time Rendering 4th Ed."
// fresnelR0�� ������ ���� ����
// Water : (0.02, 0.02, 0.02)
// Glass : (0.08, 0.08, 0.08)
// Plastic : (0.05, 0.05, 0.05)
// Gold: (1.0, 0.71, 0.29)
// Silver: (0.95, 0.93, 0.88)
// Copper: (0.95, 0.64, 0.54)
// Fresnel�� �����ϰ� �����ϴ� ��
float3 SchlickFresnel(float3 fresnelR0, float3 normal, float3 toCam)
{
    // ���� �ڷ��
    // THE SCHLICK FRESNEL APPROXIMATION by Zander Majercik, NVIDIA
    // http://psgraphics.blogspot.com/2020/03/fresnel-equations-schlick-approximation.html
    float normalDotView = saturate(dot(normal, toCam));
    
    // Rim Effect���� �����ڸ����� ������ �� ����ߴ� ���
    float f0 = 1.0f - normalDotView; // 90���� f0 = 1, 0���� f0 = 0
    
    // 1.0���� ���� ���� ���� �� ���ϸ� �� ���� ���� �ȴ�.
    // 0�� -> f0 = 0 -> fresnelR0 return, 90�� -> float3(1.0) return
    // 90���� ����� �����ڸ��� Specular ����, 0���� ����� ������ ���� ����(FresnelR0)
    return fresnelR0 + (1.0 - fresnelR0) * pow(f0, 5.0);
}

float4 psMain(PSInput input) : SV_TARGET // SV_TARGET : ����� render target�� ����
{
    float3 toCam = normalize(camWorld - input.posWorld);
    float3 color = input.color;
    
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-for
    // https://forum.unity.com/threads/what-are-unroll-and-loop-when-to-use-them.1283096/
    int i = 0;
    // �� ������ index�� �����ؼ� for loop�� �����ߵȴ�.
    // [Attribute]�� unroll, loop, fastopt ���� �ִ�.
    [unroll] // warning X3557: loop only executes for 1 iteration(s), forcing loop to unroll
    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
        color += DirectionalLight(lights[i], input.normalWorld, toCam, material);
    [unroll] 
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
        color += PointLight(lights[i], input.posWorld, input.normalWorld, toCam, material);
    [unroll] 
    for (i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
        color += SpotLight(lights[i], input.posWorld, input.normalWorld, toCam, material);
     
    // rim ���� ȿ��
    // color = RimLighting(color, toCam, input.normalWorld);
    
    // IBL�� �ٸ� Shading �Բ� ���
    // ����: https://www.shadertoy.com/view/lscBW4
    // https://www.youtube.com/watch?v=xWCZiksqCGA&ab_channel=Makin%27StuffLookGood
    
    // diffuse texture image������ ����ϴ� �̹����� Blur ȿ���� ���� �̹����� ����.
    // ���ݻ�, �̹����� �ε巴�� ó��
    float4 diffuse = g_diffuseCube.Sample(g_sampler0, input.normalWorld) + float4(color, 1.0);
    // specular�� ������ �̹��� (�ݼ�, �ſ� � ���)
    float4 specular = g_specularCube.Sample(g_sampler0, reflect(-toCam, input.normalWorld));
    
    diffuse *= float4(material.diffuse, 1.0f);
    specular *= pow((specular.x + specular.y + specular.z) / 3.0, material.shininess);
    specular *= float4(material.specular, 1.0f);
    
    /*
        Fresnel ȿ���� ������ �����̿��� �����ٺ��� ������ ǥ�鿡 �������� �����ٺ���.
        �׷��� ǥ�鿡 "����"�� �������� ������ �����ϰ� �ָ��ִ� ����� "����"�� ������ �ݻ簡 ��������.
        (���� ���� ��� X)
        �� ��� Fresnel ȿ���� Specular ��꿡 ���(������ �������� specular ����)
        Specular�� �⺻������ �ݻ籤�̶� �� ���� �ݻ� ���͸� �̿��� fresnel ȿ���� �����Ѵ�.
        ex) ���� ������ ��ü�� ���� ������, �����ڸ��ϼ��� specularȿ�� ����
        // ����: https://www.shadertoy.com/view/lscBW4
    */
    float f = SchlickFresnel(material.fresnelR0, input.normalWorld, toCam);
    specular.xyz *= f;
    
    // Texture �� ����
    if (useTexture)
    {
        diffuse *= g_texture0.Sample(g_sampler0, input.texcoord);
    }
    
    return diffuse + specular;
}
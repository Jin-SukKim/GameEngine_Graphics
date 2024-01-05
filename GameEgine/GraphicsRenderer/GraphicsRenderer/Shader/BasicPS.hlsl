#include "Commons.hlsli"

// 사용할 이미지를 GPU 메모리에서 Texture2D 형식으로 받아온다.
Texture2D g_texture0 : register(t0); // t는 texture
// Environmental Light로 사용하기 위한 CubeMap Texture
TextureCube g_textureCube : register(t1);
TextureCube g_diffuseCube : register(t2);
TextureCube g_specularCube : register(t3);
// Texture Image 안의 원하는 위치에서 색깔값을 가져오는 작업을 Sampling이라 한다.
SamplerState g_sampler0 : register(s0); // s는 sampler

cbuffer MeshPSConstData : register(b0)
{
    float3 camWorld;            // 시점의 월드 좌표계 위치
    bool useTexture;    
    Material material;          // 재질
    Light lights[MAX_LIGHTS]; // 조명
    RimLight rimLights;
};

// object의 테두리에 빛이 나는 역광 조명 효과
float3 RimLighting(float3 color, float3 toCam, float3 normal)
{
    // 모델의 중심에 가까울수록 dot product의 값은 1.0에 가까워진다. (안으로 갈수록 rim값은 0.0에 가까워진다.)
    float rim = 1.0f - dot(toCam, normal);
    
    // Smoothstep
    // https://thebookofshaders.com/glossary/?search=smoothstep
    if (rimLights.useSmoothStep)
        rim = smoothstep(0.0f, 1.0f, rim); // 내장함수 사용
    
    rim = pow(abs(rim), rimLights.rimPower);

    return color + rim * rimLights.rimColor * rimLights.rimStrength;
}

// Environment Mapping (환경 매핑)
float4 IBLReflect(float3 toCam, float3 normal)
{
    // Reflect(광선이 들어오는 방향, 노멀 벡터)
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-reflect
    // 이동에 대해서는 반영을 하면 안되기에 normal을 사용하지만 view에는 영향을 받는다.
    // 즉, 반사광을 사용해 주변 환경에 대한 색을 입힌다. 
    //(시점에서 물체의 표면의 완전 반사 방향의 색을 가져온다)
    float4 cubeColor = g_textureCube.Sample(g_sampler0, reflect(-toCam, normal));
    return cubeColor;
}

// Schlick approximation: Eq. 9.17 in "Real-Time Rendering 4th Ed."
// fresnelR0는 물질의 고유 성질
// Water : (0.02, 0.02, 0.02)
// Glass : (0.08, 0.08, 0.08)
// Plastic : (0.05, 0.05, 0.05)
// Gold: (1.0, 0.71, 0.29)
// Silver: (0.95, 0.93, 0.88)
// Copper: (0.95, 0.64, 0.54)
// Fresnel을 간단하게 구현하는 식
float3 SchlickFresnel(float3 fresnelR0, float3 normal, float3 toCam)
{
    // 참고 자료들
    // THE SCHLICK FRESNEL APPROXIMATION by Zander Majercik, NVIDIA
    // http://psgraphics.blogspot.com/2020/03/fresnel-equations-schlick-approximation.html
    float normalDotView = saturate(dot(normal, toCam));
    
    // Rim Effect에서 가장자리인지 구분할 때 사용했던 방식
    float f0 = 1.0f - normalDotView; // 90도면 f0 = 1, 0도면 f0 = 0
    
    // 1.0보다 작은 값은 여러 번 곱하면 더 작은 값이 된다.
    // 0도 -> f0 = 0 -> fresnelR0 return, 90도 -> float3(1.0) return
    // 90도에 가까운 가장자리는 Specular 색상, 0도에 가까운 안쪽은 고유 색상(FresnelR0)
    return fresnelR0 + (1.0 - fresnelR0) * pow(f0, 5.0);
}

float4 psMain(PSInput input) : SV_TARGET // SV_TARGET : 결과를 render target에 저장
{
    float3 toCam = normalize(camWorld - input.posWorld);
    float3 color = input.color;
    
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-for
    // https://forum.unity.com/threads/what-are-unroll-and-loop-when-to-use-them.1283096/
    int i = 0;
    // 각 조명의 index를 생각해서 for loop를 돌려야된다.
    // [Attribute]로 unroll, loop, fastopt 등이 있다.
    [unroll] // warning X3557: loop only executes for 1 iteration(s), forcing loop to unroll
    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
        color += DirectionalLight(lights[i], input.normalWorld, toCam, material);
    [unroll] 
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
        color += PointLight(lights[i], input.posWorld, input.normalWorld, toCam, material);
    [unroll] 
    for (i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
        color += SpotLight(lights[i], input.posWorld, input.normalWorld, toCam, material);
     
    // rim 조명 효과
    // color = RimLighting(color, toCam, input.normalWorld);
    
    // IBL과 다른 Shading 함께 사용
    // 참고: https://www.shadertoy.com/view/lscBW4
    // https://www.youtube.com/watch?v=xWCZiksqCGA&ab_channel=Makin%27StuffLookGood
    
    // diffuse texture image용으로 사용하는 이미지는 Blur 효과를 입힌 이미지와 같다.
    // 난반사, 이미지를 부드럽게 처리
    float4 diffuse = g_diffuseCube.Sample(g_sampler0, input.normalWorld) + float4(color, 1.0);
    // specular는 선명한 이미지 (금속, 거울 등에 사용)
    float4 specular = g_specularCube.Sample(g_sampler0, reflect(-toCam, input.normalWorld));
    
    diffuse *= float4(material.diffuse, 1.0f);
    specular *= pow((specular.x + specular.y + specular.z) / 3.0, material.shininess);
    specular *= float4(material.specular, 1.0f);
    
    /*
        Fresnel 효과는 수면을 가까이에서 내려다보면 수면의 표면에 수직으로 내려다본다.
        그래서 표면에 "수직"에 가까울수록 투과를 많이하고 멀리있는 수면과 "수평"에 가까우면 반사가 많아진다.
        (현재 굴절 고려 X)
        이 경우 Fresnel 효과를 Specular 계산에 사용(수직에 가까울수록 specular 감소)
        Specular는 기본적으로 반사광이라 이 완전 반사 벡터를 이용해 fresnel 효과를 구현한다.
        ex) 구의 안쪽은 물체의 색을 렌더링, 가장자리일수록 specular효과 적용
        // 참고: https://www.shadertoy.com/view/lscBW4
    */
    float f = SchlickFresnel(material.fresnelR0, input.normalWorld, toCam);
    specular.xyz *= f;
    
    // Texture 색 설정
    if (useTexture)
    {
        diffuse *= g_texture0.Sample(g_sampler0, input.texcoord);
    }
    
    return diffuse + specular;
}
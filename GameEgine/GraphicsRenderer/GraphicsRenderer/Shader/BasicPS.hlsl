#include "Commons.hlsli"

// ����� �̹����� GPU �޸𸮿��� Texture2D �������� �޾ƿ´�.
Texture2D g_texture0 : register(t0); // t�� texture
// Texture Image ���� ���ϴ� ��ġ���� ������ �������� �۾��� Sampling�̶� �Ѵ�.
SamplerState g_sampler0 : register(s0); // s�� sampler

cbuffer MeshPSConstData : register(b0)
{
    float3 camWorld;            // ������ ���� ��ǥ�� ��ġ
    bool useTexture;    
    Material material;          // ����
    Light lights[MAX_LIGHTS]; // ����
};

// Pixel Shader Input ����ü
struct PSInput
{
    float4 posProj : SV_POSITION;   // screen ��ǥ���� ��ġ
    float3 posWorld : POSITION;     // World ��ǥ���� ��ġ (���� ��꿡 ���)
    float3 normalWorld : NORMAL;    // World ��ǥ���� Normal 
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

float4 psMain(PSInput input) : SV_TARGET
{
    float3 toCam = normalize(camWorld - input.posWorld);
    float3 color = input.color;
    
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-for
    // https://forum.unity.com/threads/what-are-unroll-and-loop-when-to-use-them.1283096/
    int i = 0;
    // �� ������ index�� �����ؼ� for loop�� �����ߵȴ�.
    [unroll] // warning X3557: loop only executes for 1 iteration(s), forcing loop to unroll
    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
        color += DirectionalLight(lights[i], input.normalWorld, toCam, material);
    [unroll] 
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
        color += PointLight(lights[i], input.posWorld, input.normalWorld, toCam, material);
    [unroll] 
    for (i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
        color += SpotLight(lights[i], input.posWorld, input.normalWorld, toCam, material);
     
    return useTexture ? float4(color, 1.0) * g_texture0.Sample(g_sampler0, input.texcoord) : float4(color, 1.0f);
}
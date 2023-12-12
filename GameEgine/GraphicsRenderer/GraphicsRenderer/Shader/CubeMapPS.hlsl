TextureCube g_textureCube0 : register(t0);
SamplerState g_sampler : register(s0);

// Pixel Shader Input 구조체
struct PSInput
{
    float4 posProj : SV_POSITION; // screen 좌표계의 위치
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
};

float4 psMain(PSInput input) : SV_TARGET
{
    return g_textureCube0.Sample(g_sampler, input.posWorld.xyz);
}
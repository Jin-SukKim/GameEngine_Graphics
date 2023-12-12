TextureCube g_textureCube0 : register(t0);
SamplerState g_sampler : register(s0);

// Pixel Shader Input ����ü
struct PSInput
{
    float4 posProj : SV_POSITION; // screen ��ǥ���� ��ġ
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
};

float4 psMain(PSInput input) : SV_TARGET
{
    return g_textureCube0.Sample(g_sampler, input.posWorld.xyz);
}
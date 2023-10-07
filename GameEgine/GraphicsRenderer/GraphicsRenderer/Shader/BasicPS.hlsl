#include "Commons.hlsli"

// ����� �̹����� GPU �޸𸮿��� Texture2D �������� �޾ƿ´�.
Texture2D g_texture0 : register(t0); // t�� texture
// Texture Image ���� ���ϴ� ��ġ���� ������ �������� �۾��� Sampling�̶� �Ѵ�.
SamplerState g_sampler0 : register(s0); // s�� sampler

cbuffer PixelConstantBuffer : register(b0)
{
    float3 camWorld;            // ������ ���� ��ǥ�� ��ġ
    bool useTexture;    
    Material material;          // ����
    Light light[MAX_LIGHTS];    // ����
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
    //return float4(input.color, 1.0);
    return g_texture0.Sample(g_sampler0, input.texcoord);
}
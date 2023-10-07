#include "Commons.hlsli"

// 사용할 이미지를 GPU 메모리에서 Texture2D 형식으로 받아온다.
Texture2D g_texture0 : register(t0); // t는 texture
// Texture Image 안의 원하는 위치에서 색깔값을 가져오는 작업을 Sampling이라 한다.
SamplerState g_sampler0 : register(s0); // s는 sampler

cbuffer PixelConstantBuffer : register(b0)
{
    float3 camWorld;            // 시점의 월드 좌표계 위치
    bool useTexture;    
    Material material;          // 재질
    Light light[MAX_LIGHTS];    // 조명
};

// Pixel Shader Input 구조체
struct PSInput
{
    float4 posProj : SV_POSITION;   // screen 좌표계의 위치
    float3 posWorld : POSITION;     // World 좌표계의 위치 (조명 계산에 사용)
    float3 normalWorld : NORMAL;    // World 좌표계의 Normal 
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

float4 psMain(PSInput input) : SV_TARGET
{
    //return float4(input.color, 1.0);
    return g_texture0.Sample(g_sampler0, input.texcoord);
}
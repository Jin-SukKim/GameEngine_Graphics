// Vertex Shader Input 구조체
struct VSInput
{
    float3 posWorld : POSITION;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
};

// Pixel Shader Input 구조체
struct PSInput
{
    float4 posProj : SV_POSITION; // screen 좌표계의 위치
    float3 posWorld : POSITION;
};

// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer는 register b 사용
{
    matrix viewProj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    output.posWorld = input.posWorld;
    output.posProj = mul(float4(input.posWorld, 1.0f), viewProj);
    
    return output;
}
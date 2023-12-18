// Vertex Shader Input 구조체
struct VSInput
{
    float3 posModel : POSITION;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
};

// Pixel Shader Input 구조체
struct PSInput
{
    float4 posProj : SV_POSITION; // screen 좌표계의 위치
    float3 posModel : POSITION;
};

// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer는 register b 사용
{
    matrix viewProj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    output.posModel = input.posModel; // 모델 좌표계
    output.posProj = mul(float4(input.posModel, 1.0f), viewProj); // 스크린 좌표계로 변환
    
    return output;
}
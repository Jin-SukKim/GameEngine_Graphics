// Vertex Shader Input ����ü
struct VSInput
{
    float3 posModel : POSITION;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
};

// Pixel Shader Input ����ü
struct PSInput
{
    float4 posProj : SV_POSITION; // screen ��ǥ���� ��ġ
    float3 posModel : POSITION;
};

// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer�� register b ���
{
    matrix viewProj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    output.posModel = input.posModel; // �� ��ǥ��
    output.posProj = mul(float4(input.posModel, 1.0f), viewProj); // ��ũ�� ��ǥ��� ��ȯ
    
    return output;
}
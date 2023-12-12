// Vertex Shader Input ����ü
struct VSInput
{
    float3 posWorld : POSITION;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
};

// Pixel Shader Input ����ü
struct PSInput
{
    float4 posProj : SV_POSITION; // screen ��ǥ���� ��ġ
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
};

// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer�� register b ���
{
    matrix viewProj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    /*
    float4 pos = float4(input.posWorld, 1.0);
    pos = mul(pos, world);
    
    output.posWorld = pos.xyz;
    
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.posProj = pos;
    output.texcoord = input.texcoord;
    
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    */
    return output;
}
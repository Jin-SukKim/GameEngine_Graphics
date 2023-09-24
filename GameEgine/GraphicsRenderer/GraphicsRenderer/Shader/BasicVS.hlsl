// Constant buffer
cbuffer MeshConstantBuffer : register(b0) // buffer�� register b ���
{
    matrix world;
    matrix view;
    matrix proj;
};


// Vertex Shader Input ����ü
struct VSInput
{
    float3 posWorld : POSITION;
    float3 color : COLOR0;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
};

// Vertex Shader Output ����ü
// �ٸ� Shader�� ������� �ʰ� �����Ƿ� Pixel Shader�� Input ����ü
struct PSInput
{
    float4 posProj : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

PSInput VSmain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.posWorld, 1.0);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.posProj = pos;
    output.color = input.color;
    
    return output;
}
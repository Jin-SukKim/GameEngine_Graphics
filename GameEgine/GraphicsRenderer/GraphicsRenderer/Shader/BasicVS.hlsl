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
    float3 pos : POSITION;
    float3 color : COLOR0;
};

// Vertex Shader Output ����ü
// �ٸ� Shader�� ������� �ʰ� �����Ƿ� Pixel Shader�� Input ����ü
struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

PSInput VSmain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.pos, 1.0);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.pos = pos;
    output.color = input.color;
    
    return output;
}
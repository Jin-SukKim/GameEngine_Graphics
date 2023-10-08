// Constant buffer
cbuffer MeshConstantBuffer : register(b0) // buffer�� register b ���
{
    matrix world;
    matrix invTranspose; 
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
    float3 posWorld : POSITION;
    float3 normalWorld : NORMAL;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.posWorld, 1.0);
    pos = mul(pos, world);
    
    output.posWorld = pos;
    
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.posProj = pos;
    output.color = float3(0.0, 0.0, 0.0);
    output.texcoord = input.texcoord;
    
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    return output;
}
// Pixel Shader Input ����ü
struct VSOutput
{
    float4 posProj : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

float4 psMain(VSOutput input) : SV_Target
{
    return float4(input.color, 1.0);
}
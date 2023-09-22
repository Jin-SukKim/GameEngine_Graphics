// Pixel Shader Input ±¸Á¶Ã¼
struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float4 PSmain(VSOutput input) : SV_Target
{
    return float4(input.color, 1.0);
}
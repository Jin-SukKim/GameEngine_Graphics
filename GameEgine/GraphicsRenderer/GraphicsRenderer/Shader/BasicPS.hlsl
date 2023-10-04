// ����� �̹����� GPU �޸𸮿��� Texture2D �������� �޾ƿ´�.
Texture2D g_texture0 : register(t0); // t�� texture
// Texture Image ���� ���ϴ� ��ġ���� ������ �������� �۾��� Sampling�̶� �Ѵ�.
SamplerState g_sampler0 : register(s0); // s�� sampler

// Pixel Shader Input ����ü
struct VSOutput
{
    float4 posProj : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

float4 psMain(VSOutput input) : SV_TARGET
{
    return float4(input.color, 1.0);
    // return g_texture0.Sample(g_sampler0, input.texcoord);
}
TextureCube g_textureCube0 : register(t0);
SamplerState g_sampler : register(s0);

// Pixel Shader Input ����ü
struct PSInput
{
    float4 posProj : SV_POSITION; // screen ��ǥ���� ��ġ
    float3 posModel : POSITION;
};

float4 psMain(PSInput input) : SV_TARGET
{
    // texture ��ǥ�� vertex ������ �ƴ϶� pixel ������ ����� mesh�� ���� �� texture�� ����� �������� �Ѵ�.
    // sphere mapping(�� ����)
    // �׷��� Vertex Shader���� interpolation�� ���� ��ǥ�踦 ����ϸ�
    // pixel shader�� �׷��� �� ���� ��ǥ���� ��Ȯ�� �˰� �����Ƿ� 
    // model ��ǥ���� ���� ����ϸ� �ȴ�.
    // ������ texture ��ǥ��� vertex ������ interpolation�� ���̱⿡
    // ���� ������ �κп��� �ǵ����� �ʾҴ� uv���� ������ ���̴�.
    
    // CubeMap�� ���� ������ Sample()�� �ʿ��ϴ�. (ex: �߾ӿ��� CubeMap�� ���� ���� ����)
    // (�̷������δ� Normalize�ϴ°� ������ �����δ� ���̰� ���� ����. �Ƹ� GPU Driver���� ����ȭ�� ������ ������?)
    return g_textureCube0.Sample(g_sampler, input.posModel);
}
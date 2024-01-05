TextureCube g_textureCube0 : register(t0);
SamplerState g_sampler : register(s0);

// Pixel Shader Input 구조체
struct PSInput
{
    float4 posProj : SV_POSITION; // screen 좌표계의 위치
    float3 posModel : POSITION;
};

float4 psMain(PSInput input) : SV_TARGET
{
    // texture 좌표를 vertex 단위가 아니라 pixel 단위로 계산해 mesh가 구일 떄 texture가 제대로 입혀지게 한다.
    // sphere mapping(구 매핑)
    // 그래서 Vertex Shader에서 interpolation된 모델의 좌표계를 사용하면
    // pixel shader는 그려야 될 모델의 좌표값을 정확히 알고 있으므로 
    // model 좌표계의 값을 사용하면 된다.
    // 하지만 texture 좌표계는 vertex 단위로 interpolation된 값이기에
    // 구의 이음새 부분에서 의도하지 않았던 uv값이 나오는 것이다.
    
    // CubeMap을 보는 방향이 Sample()에 필요하다. (ex: 중앙에서 CubeMap을 향해 보는 방향)
    // (이론적으로는 Normalize하는게 맞으나 실제로는 차이가 거의 없다. 아마 GPU Driver에서 최적화를 해주지 않을까?)
    return g_textureCube0.Sample(g_sampler, input.posModel);
}
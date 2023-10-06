// Shader 파일에서 Include 가능한 파일

struct Light
{
    float3 pos;
    float fallOffStart;
    float3 dir;
    float fallOffEnd;
    float3 strength;
    float spotPower;
    float LightType;
};

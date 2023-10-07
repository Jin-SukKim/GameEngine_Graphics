// Shader 파일에서 Include 가능한 파일 
// - (단, shader 프로그램을 실행시키는 함수가 있으면 안된다(ex: main)
// 공통되는 부분이나 여러 Shader에서 공통적으로 사용하는 함수를 작성

#define MAX_LIGHTS 3

// 조명
struct Light
{
    float3 pos;
    float fallOffStart;
    float3 dir;
    float fallOffEnd;
    float3 strength;
    float spotPower;
    float lightType;
};

// 재질
struct Material
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    float dummy1;
    float3 specular;
    float dummy2;
    
};

float3 BlinnPhong(float3 lightDir, float3 normal, float3 toCam,
    float3 lightStrength, Material mat)
{    
    // Specular
    float3 halfWay = normalize(lightDir + toCam);
    // 90도가 넘으면 음수가 되는데 빛은 아무리 어두워도 0이 되도록 한다.
    float hdotn = max(dot(halfWay, normal), 0.0f);
    float3 specular = mat.specular * pow(hdotn, mat.shininess);
    
    // Ambient color : 빛이 없어도 보이는 물체가 가진 고유의 색
    // Diffuse Color : 표면의 밝기에 따라 물체의 색의 강도 조절 (표면에서 분산되는 빛)
    // Specular Color : 재질에 따라 빛의 집중도가 달라진다 (일종의 반사광)
    return mat.ambient + (mat.diffuse + specular) * lightStrength;
}


float3 DirectionalLight(Light L, float3 normal, float3 toCam, Material mat)
{
    float3 lightDir = -L.dir;

    // 표면과 빛의 각도
    float ndotl = max(dot(normal, lightDir), 0.0f);
    // 표면 밝기 - 각도가 90도를 넘으면 어두워져서 보이지 않는다.
    float3 lightStrength = L.strength * ndotl;

    return BlinnPhong(lightDir, normal, toCam, lightStrength, mat);
}

// 조명의 강도 계산을 위해 사용
// 물체가 조명으로부터 멀어지면 점점 어두워진다. (최적화 방안)
float CalAttenuation(float d, float fallOfStart, float fallOfEnd)
{
    // 빛의 중심으로부터 멀어질수록 d(거리)가 늘어난다. (빛의 중심 d = 0.f)
    return saturate((fallOfEnd - d) / (fallOfEnd - fallOfStart)); // 0.f ~ 1.f 범위
}

float3 PointLight(Light L, float3 pos, float3 normal, float3 toCam, Material mat)
{
    float3 lightDir = L.pos - pos;
    // 렌더링할 Point와 조명 사이의 거리
    float d = length(lightDir);
    
    // 모델과 조명이 너무 멀면 조명이 적용되지 않는다.
    if (d > L.fallOffEnd)
    {
        return float3(0.f, 0.f, 0.f);
    }
    else
    {
        // 빛의 방향 벡터
        lightDir /= d;
        
        // 표면과 빛의 각도
        float ndotl = max(dot(normal, lightDir), 0.0f);
        // 빛의 밝기 - 각도가 90도를 넘으면 어두워져서 보이지 않는다.
        float3 lightStrength = L.strength * ndotl;
        
        float att = CalAttenuation(d, L.fallOffStart, L.fallOffEnd);
        // 밝기
        lightStrength *= att;
        
        return BlinnPhong(lightDir, normal, toCam, lightStrength, mat);
    }
}

float3 SpotLight(Light L, float3 pos, float3 normal, float3 toCam, Material mat)
{
    float3 lightDir = L.pos - pos;
    
    float d = length(lightDir);
    
    if (d > L.fallOffEnd)
    {
        return float3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        // 빛의 방향 벡터
        lightDir /= d;
        
        float ndotl = max(dot(normal, lightDir), 0.0f);
        float3 lightStrength = L.strength * ndotl;
        
        float att = CalAttenuation(d, L.fallOffStart, L.fallOffEnd);
        lightStrength *= att;
        
        // Point Light는 조명의 전방 방향만 비춘다.
        // 조명의 방향과 렌더링한 위치에서 조명으로의 방향을 가지고 계산
        float spotFactor = pow(max(dot(L.dir, -lightDir), 0.0f), L.spotPower);
        lightStrength *= spotFactor;
        
        return BlinnPhong(lightDir, normal, toCam, lightStrength, mat);
    }

}



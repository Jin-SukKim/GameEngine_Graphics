// Shader ���Ͽ��� Include ������ ���� 
// - (��, shader ���α׷��� �����Ű�� �Լ��� ������ �ȵȴ�(ex: main)
// ����Ǵ� �κ��̳� ���� Shader���� ���������� ����ϴ� �Լ��� �ۼ�

#define MAX_LIGHTS 3

// ����
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

// ����
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
    // 90���� ������ ������ �Ǵµ� ���� �ƹ��� ��ο��� 0�� �ǵ��� �Ѵ�.
    float hdotn = max(dot(halfWay, normal), 0.0f);
    float3 specular = mat.specular * pow(hdotn, mat.shininess);
    
    // Ambient color : ���� ��� ���̴� ��ü�� ���� ������ ��
    // Diffuse Color : ǥ���� ��⿡ ���� ��ü�� ���� ���� ���� (ǥ�鿡�� �л�Ǵ� ��)
    // Specular Color : ������ ���� ���� ���ߵ��� �޶����� (������ �ݻ籤)
    return mat.ambient + (mat.diffuse + specular) * lightStrength;
}


float3 DirectionalLight(Light L, float3 normal, float3 toCam, Material mat)
{
    float3 lightDir = -L.dir;

    // ǥ��� ���� ����
    float ndotl = max(dot(normal, lightDir), 0.0f);
    // ǥ�� ��� - ������ 90���� ������ ��ο����� ������ �ʴ´�.
    float3 lightStrength = L.strength * ndotl;

    return BlinnPhong(lightDir, normal, toCam, lightStrength, mat);
}

// ������ ���� ����� ���� ���
// ��ü�� �������κ��� �־����� ���� ��ο�����. (����ȭ ���)
float CalAttenuation(float d, float fallOfStart, float fallOfEnd)
{
    // ���� �߽����κ��� �־������� d(�Ÿ�)�� �þ��. (���� �߽� d = 0.f)
    return saturate((fallOfEnd - d) / (fallOfEnd - fallOfStart)); // 0.f ~ 1.f ����
}

float3 PointLight(Light L, float3 pos, float3 normal, float3 toCam, Material mat)
{
    float3 lightDir = L.pos - pos;
    // �������� Point�� ���� ������ �Ÿ�
    float d = length(lightDir);
    
    // �𵨰� ������ �ʹ� �ָ� ������ ������� �ʴ´�.
    if (d > L.fallOffEnd)
    {
        return float3(0.f, 0.f, 0.f);
    }
    else
    {
        // ���� ���� ����
        lightDir /= d;
        
        // ǥ��� ���� ����
        float ndotl = max(dot(normal, lightDir), 0.0f);
        // ���� ��� - ������ 90���� ������ ��ο����� ������ �ʴ´�.
        float3 lightStrength = L.strength * ndotl;
        
        float att = CalAttenuation(d, L.fallOffStart, L.fallOffEnd);
        // ���
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
        // ���� ���� ����
        lightDir /= d;
        
        float ndotl = max(dot(normal, lightDir), 0.0f);
        float3 lightStrength = L.strength * ndotl;
        
        float att = CalAttenuation(d, L.fallOffStart, L.fallOffEnd);
        lightStrength *= att;
        
        // Point Light�� ������ ���� ���⸸ �����.
        // ������ ����� �������� ��ġ���� ���������� ������ ������ ���
        float spotFactor = pow(max(dot(L.dir, -lightDir), 0.0f), L.spotPower);
        lightStrength *= spotFactor;
        
        return BlinnPhong(lightDir, normal, toCam, lightStrength, mat);
    }

}



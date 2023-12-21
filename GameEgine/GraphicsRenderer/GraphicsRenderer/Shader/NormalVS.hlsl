#include "Commons.hlsli"
// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer�� register b ���
{
    matrix model;
    matrix invTranspose;
    matrix viewProj;
};

cbuffer MeshNormalConstData : register(b1)
{
    float scale;
    float dummy[3];
};


PSInput vsMain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.posModel, 1.0);
    pos = mul(pos, model); // ���� ��ǥ��� ��ȯ
    
    // Normal Vector�� ������ �˾ƾ� ������ ������ �׸� �� �ִ�.
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz; // Normal Vector ������
    output.normalWorld = normalize(output.normalWorld);
    
    // ���� normal�� ������ �������� ���� ��ǥ�� �����Ƿ�
    // texcoord�� 0.0, 1.0���� �ٸ� �� �̿��� �������� ��ȯ
    float t = input.texcoord.x;
    
    // ������ texcoord = 0.0, ���� = 1.0���� �������� ���� vertex ��ġ�� ������
    // ������ normal vector �������� scale ��ŭ �̵��� ��ġ�� ��ȯ
    pos.xyz += output.normalWorld * t * scale;
    
    output.posWorld = pos.xyz; // ���� ��ǥ��
    
    pos = mul(pos, viewProj);
    
    output.posProj = pos;
    output.texcoord = input.texcoord;
    
    // �������� ������̰� �������� ������ �������� �׶��̼�
    output.color = float3(1.0, 1.0, 0.0) * (1.0 - t) + float3(1.0, 0.0, 0.0) * t;

    return output;
}
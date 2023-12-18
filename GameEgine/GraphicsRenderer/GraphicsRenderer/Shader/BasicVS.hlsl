#include "Commons.hlsli"
// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer�� register b ���
{
    matrix model;
    matrix invTranspose; 
    //matrix view;
    //matrix proj;
    matrix viewProj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.posModel, 1.0); // �� ��ǥ��
    pos = mul(pos, model);  // �� ����� ���� ���� ��ǥ��� ��ȯ
    
    output.posWorld = pos.xyz; // ���� ��ǥ��

    // pos = mul(pos, view); // �� ��ǥ��� ��ȯ
    // pos = mul(pos, proj); // ��ũ�� ��ǥ��� ��ȯ
 
    pos = mul(pos, viewProj);
    
    output.posProj = pos;
    output.color = float3(0.0, 0.0, 0.0);
    output.texcoord = input.texcoord;
    
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    return output;
}
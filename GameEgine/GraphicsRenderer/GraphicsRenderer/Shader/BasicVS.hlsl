#include "Commons.hlsli"
// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer는 register b 사용
{
    matrix world;
    matrix invWorld; 
    //matrix view;
    //matrix proj;
    matrix viewProj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.posModel, 1.0); // 모델 좌표계
    pos = mul(pos, world); // 모델 행렬을 곱해 월드 좌표계로 변환
    
    output.posWorld = pos.xyz; // 월드 좌표계

    // pos = mul(pos, view); // 뷰 좌표계로 변환
    // pos = mul(pos, proj); // 스크린 좌표계로 변환
 
    pos = mul(pos, viewProj);
    
    output.posProj = pos;
    output.color = float3(0.0, 0.0, 0.0);
    output.texcoord = input.texcoord;
    
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invWorld).xyz; // 월드 좌표계에서 모델 좌표계로 변환
    output.normalWorld = normalize(output.normalWorld);
    
    return output;
}
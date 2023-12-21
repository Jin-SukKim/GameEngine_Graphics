#include "Commons.hlsli"
// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer는 register b 사용
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
    pos = mul(pos, model); // 월드 좌표계로 변환
    
    // Normal Vector의 방향을 알아야 선분의 끝점을 그릴 수 있다.
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz; // Normal Vector 재조정
    output.normalWorld = normalize(output.normalWorld);
    
    // 현재 normal의 끝점과 시작점은 같은 좌표에 있으므로
    // texcoord가 0.0, 1.0으로 다른 걸 이용해 선분으로 변환
    float t = input.texcoord.x;
    
    // 시작점 texcoord = 0.0, 끝점 = 1.0으로 시작점은 원래 vertex 위치를 가지고
    // 끝점은 normal vector 방향으로 scale 만큼 이동한 위치로 변환
    pos.xyz += output.normalWorld * t * scale;
    
    output.posWorld = pos.xyz; // 월드 좌표계
    
    pos = mul(pos, viewProj);
    
    output.posProj = pos;
    output.texcoord = input.texcoord;
    
    // 시작점은 노란색이고 끝점으로 갈수록 빨간색인 그라데이션
    output.color = float3(1.0, 1.0, 0.0) * (1.0 - t) + float3(1.0, 0.0, 0.0) * t;

    return output;
}
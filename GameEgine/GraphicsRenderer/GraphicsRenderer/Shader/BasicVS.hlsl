#include "Commons.hlsli"
// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer는 register b 사용
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
    
    // point 이기에 맨 뒤에 1.0을 붙여준다. (Homogeneous Coordinate)
    float4 pos = float4(input.posModel, 1.0); // 모델 좌표계
    pos = mul(pos, model); // 모델 행렬을 곱해 월드 좌표계로 변환
    
    // 보내는 데이터 사이즈를 줄이기 위해 3차원 좌표로 보내준다. 
    // 그리고 이동, 회전, 크기 변환을 한번에 하기 위해 동차좌표계를 사용해 Affine Transformation을
    // 했지만 현재 pixel shader에선 변환하지 않기에 3차원 좌표만 보내도 충분하다.
    output.posWorld = pos.xyz; // 월드 좌표계

    // pos = mul(pos, view); // 뷰 좌표계로 변환
    // pos = mul(pos, proj); // 스크린 좌표계로 변환
 
    pos = mul(pos, viewProj);
    
    output.posProj = pos;
    output.color = float3(0.0, 0.0, 0.0);
    output.texcoord = input.texcoord;
    
    // vector이기에 맨 뒤에 0.0을 붙여준다.
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz; // normal vector를 표면에 수직하도록 변환
    output.normalWorld = normalize(output.normalWorld);
    
    return output;
}
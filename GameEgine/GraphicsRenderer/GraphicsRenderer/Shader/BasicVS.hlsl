#include "Commons.hlsli"
// Constant buffer
cbuffer MeshVSConstData : register(b0) // buffer´Â register b »ç¿ë
{
    matrix world;
    matrix invTranspose; 
    matrix view;
    matrix proj;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.posWorld, 1.0);
    pos = mul(pos, world);
    
    output.posWorld = pos.xyz;
    
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.posProj = pos;
    output.color = float3(0.0, 0.0, 0.0);
    output.texcoord = input.texcoord;
    
    float4 normal = float4(input.normalWorld, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    return output;
}
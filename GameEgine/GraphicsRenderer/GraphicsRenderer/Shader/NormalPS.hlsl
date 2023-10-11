#include "Commons.hlsli"

float4 psMain(PSInput input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}
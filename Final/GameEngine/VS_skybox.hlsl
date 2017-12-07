#include "Common.hlsli"

VSkyboxToPixel main(VPosColNormInput input)
{
	VSkyboxToPixel output;

    output.position = mul(float4(input.position, 1), mul(objectTransform, viewProjection)).xyww;
     
    output.localPos = input.position;

    return output;
}
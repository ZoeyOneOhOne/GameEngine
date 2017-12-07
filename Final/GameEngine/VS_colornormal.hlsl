#include "Common.hlsli"

VPosColNormToPixel main(VPosColNormInput input)
{
	VPosColNormToPixel output;

	output.position = mul(float4(input.position, 1), mul(objectTransform, viewProjection));
	output.color = objectColor;
	output.normal = normalize(mul(input.normal, (float3x3)objectTransform));
	//output.normal = normalize(mul(input.normal, (float3x3)objectInverseTranspose));
	output.worldPos = mul(float4(input.position, 1), objectTransform).xyz;

	return output;
}
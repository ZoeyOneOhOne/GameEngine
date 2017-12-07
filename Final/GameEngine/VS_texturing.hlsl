#include "Common.hlsli"

VPosTexNormToPixel main(VPosTexNormInput input)
{
	VPosTexNormToPixel output;
	
	output.position = mul(float4(input.position, 1), mul(objectTransform, viewProjection));
	output.texCoords = input.texCoords;
	output.normal = mul(input.normal, (float3x3)objectInverseTranspose);
	output.worldPos = mul(float4(input.position, 1.0f), objectTransform).xyz;

	return output;
}
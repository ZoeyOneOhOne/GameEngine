#include "Common.hlsli"

StandardVertToPixel main(StandardVertInput input)
{
	StandardVertToPixel output;

	output.position = mul(float4(input.position, 1), mul(objectTransform, viewProjection));
	output.texCoords = input.texCoords;
	output.normal = normalize(mul(input.normal, (float3x3)objectInverseTranspose));
	output.tangent = mul(input.tangent, (float3x3)objectTransform);
	output.worldPos = mul(float4(input.position, 1.0f), objectTransform).xyz;

	return output;
}
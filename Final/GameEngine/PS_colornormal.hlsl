#include "Common.hlsli"

TextureCube skyboxTexture : register(t0);
SamplerState standardSampler : register(s0);

float4 main(VPosColNormToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float3 eyeDir = normalize(cameraPosition - input.worldPos); 
	float3 skyColor = skyboxTexture.Sample(standardSampler, reflect(-eyeDir, input.normal));	

    float3 reflection = reflect(normalize(lightDirection), input.normal);
    reflection = normalize(reflection);
    float specularity = pow(saturate(dot(reflection, eyeDir)), 8);
       
    //Normal Dot Product Light Direction
    float NdotL = dot(-input.normal, lightDirection);
    NdotL = saturate(NdotL);//Clamps a value to a range of 0 to 1

	return float4((skyColor + input.color.rgb + specularity) / 2 * NdotL + .025f, input.color.a);
}
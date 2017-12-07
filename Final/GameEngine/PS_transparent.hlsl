#include "Common.hlsli"

TextureCube skyboxTexture : register(t0);
SamplerState standardSampler : register(s0);

float4 main(VPosColNormToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float3 eyeDir = normalize(cameraPosition - input.worldPos); 
	
	//Use reflection only
	//float3 skyColor = skyboxTexture.Sample(standardSampler, reflect(-eyeDir, input.normal));
	
	//Use refraction only
	//float3 skyColor = skyboxTexture.Sample(standardSampler, refract(-eyeDir, input.normal, .66f));//.66 = 1.00029 / 1.52 = refraction index of air / index of glass

	//Combine reflection and refraction
	float3 skyColor = skyboxTexture.Sample(standardSampler, reflect(-eyeDir, input.normal));
	skyColor += skyboxTexture.Sample(standardSampler, refract(-eyeDir, input.normal, .4f));//.66 = 1.00029 / 1.52 = refraction index of air / index of glass
	skyColor /= 2;//add reflection and refraction together and average.

	//return float4(skyColor, input.color.a);//See skycolor alone, no tinting/lighting

    float3 reflection = reflect(normalize(lightDirection), input.normal);
    reflection = normalize(reflection);
    float specularity = pow(saturate(dot(reflection, eyeDir)), 8);
       
    //Normal Dot Product Light Direction
    float NdotL = dot(-input.normal, lightDirection);
    NdotL = saturate(NdotL);//Clamps a value to a range of 0 to 1 

	input.color.rgb *= input.color.a;//Multiplicative Blending

	float3 glassColor = clamp(skyColor * input.color.rgb + specularity + .025f, 0, 1);
	float glassAlpha = objectColor.a * clamp(input.color.a + specularity, 0, 1);
	return float4(glassColor, glassAlpha);
}
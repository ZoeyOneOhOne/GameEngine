#include "Common.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D specTexture : register(t1);
Texture2D normalMap : register(t2);

SamplerState standardSampler : register(s0);

float4 main(StandardVertToPixel input) : SV_TARGET
{
	input.texCoords *= textureScale;

	//Pull values from input textures
	float4 color = diffuseTexture.Sample(standardSampler, input.texCoords);
	float4 specValue = specTexture.Sample(standardSampler, input.texCoords);
	specValue = float4(1, 1, 1, 1);//disables spec mapping for demo
	float4 normalMapValue = normalMap.Sample(standardSampler, input.texCoords);

	//Masking Transparency
	clip(color.a - .1f);

	//Normalize normal which is blended across vertices, causing innaccuracies
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	//Normal Mapping 
	//input.normal = normalize(input.normal + normalMapValue);//Incorrect Normal Mapping - to show why we need the extra Tangent vector.
	float3 normalTangentSpace = 2.0f * normalMapValue - 1.0f;//Transforms normal from 0-1 to -1-1 range
	float3 N = input.normal;
	float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
	float3 B = cross(N, T);//Calculate BiTangent
	float3x3 normalTransformMatrix = float3x3(T, B, N);
	input.normal = mul(normalTangentSpace, normalTransformMatrix);//Correct Normal Mapping

	//Directional Light Reflections
	float3 eyeDir = normalize(cameraPosition - input.worldPos);
	float3 reflection = reflect(normalize(lightDirection), input.normal);
	reflection = normalize(reflection);
	float specularity = pow(saturate(dot(reflection, eyeDir)), 128) * specValue;
	specularity = max(specularity, 0);

	//Directional Light Shading
	float NdotL = dot(-input.normal, lightDirection);
	NdotL = saturate(NdotL);

	//Point Light Shading
	float3 lightVec = pointLightPos - input.worldPos;
	float d = length(lightVec);//Clamps a value to a range of 0 to 1



	if (d > pointLightRange)

		return float4(color.rgb * NdotL + .025f + specularity, color.a);

	lightVec /= d;//Normalizes the lightVec 
	float att = 1.0f / dot(pointLightAttenuation, float3(1.0f, d, d*d));
	float pointLightNdotL = dot(lightVec, input.normal);//diffuseFactor in the book
	pointLightNdotL = saturate(pointLightNdotL);//Clamps a value to a range of 0 to 1
	

	//Point Light Reflections
	[flatten]
	if (pointLightNdotL > 0.0f)
	{
		float3 v = reflect(-lightVec, input.normal);
		float pointLightSpecFactor = pow(saturate(dot(v, eyeDir)), 128) * specValue;

		//Add point light lighting to directional light lighting
		NdotL += pointLightNdotL * pointLightIntensity * att;
		specularity += pointLightSpecFactor * att;

		float3 pointLightColor= float3(0,0,1);
		float3 directionLightColor;
		directionLightColor = float3(NdotL, NdotL, NdotL);
		float3 finalColor = directionLightColor + pointLightColor;
		color.xyz += finalColor * pointLightNdotL * att;
	} 

	return float4(color.rgb * NdotL + .025f + specularity, color.a);


	

}
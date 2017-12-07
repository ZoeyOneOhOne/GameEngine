#include "Common.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D specTexture : register(t1);
SamplerState standardSampler : register(s0);

float4 main(VPosTexNormToPixel input) : SV_TARGET
{
	input.texCoords.x += gameTime;//Animate a texture by changing the UV coordinates, like for a river
	input.texCoords *= textureScale;//Scale texture to tile it (such as for a large object, like landscapes)
									//return float4(input.texCoords, 0, 1);//output the texture coords to see how they look

float4 color = diffuseTexture.Sample(standardSampler, input.texCoords);
//return float4(color, 1);//output just the color from the texture ('flat' lighting)

//Masked transparency (Binary transparency) either draw it or not.
clip(color.a - .1f);//Clips (just skips rendering) any pixel if the value goes < 0.

input.normal = normalize(input.normal);
//return float4(input.normal, 1);//output just the normals to visualize them. Remember any negatives will just be black

float3 eyeDir = normalize(cameraPosition - input.worldPos);
float3 reflection = reflect(normalize(lightDirection), input.normal);
reflection = normalize(reflection);
float specularity = pow(saturate(dot(reflection, eyeDir)), 128) * specTexture.Sample(standardSampler, input.texCoords);
specularity = max(specularity, 0);//Directional Light Specularity (reflection/highlights)

								  //Normal Dot Product Light Direction
float NdotL = dot(-input.normal, lightDirection);//Calculate Directional light lighting
NdotL = saturate(NdotL);//Clamps a value to a range of 0 to 1

						//Add in a Point Light/////////////////////////////////////////////////////////////////////////////
float3 lightVec = pointLightPos - input.worldPos;//Calculate the vector between the point light and this pixel in world space.
float d = length(lightVec);//distance between the light and the pixel	 

if (d > pointLightRange)//we don't flatten this if statement because it immediately ends the pixel shader if its true - there is nothing to optimize
return float4(color.rgb * NdotL + .025f + specularity, color.a);//Lighting just with a directional light

lightVec /= d;//Normalizes the lightVec 

			  //Attenuation - how much the intensity of the light weakens over distance
			  //In real life, it is based on the inverse of the square of the distance (use att = (0, 0, 1))
			  //Sometimes we use inverse distance for a different effect, that would be att = (0, 1, 0).
			  //See page 289 in our book.
float att = 1.0f / dot(pointLightAttenuation, float3(1.0f, d, d*d));

float pointLightNdotL = dot(lightVec, input.normal);//diffuseFactor in the book
pointLightNdotL = saturate(pointLightNdotL);

[flatten] //Graphics Card cores aren't good at if statements or loops, so 'flattening' means optimizing those for GPUs
if (pointLightNdotL > 0.0f)
{
	float3 v = reflect(-lightVec, input.normal); //calculate specularity of the point light
	float pointLightSpecFactor = pow(max(dot(v, eyeDir), 0.0f), 128);

	//Add point light lighting to directional light lighting
	NdotL += pointLightNdotL * pointLightIntensity * att;
	specularity += pointLightSpecFactor * att;
}

return float4(color.rgb * NdotL + .025f + specularity, color.a); //Lighting just with both light's added together
}
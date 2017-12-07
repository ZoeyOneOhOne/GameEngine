#include "Common.hlsli"

TextureCube skyboxTexture : register(t0);
SamplerState standardSampler : register(s0);

float4 main(VSkyboxToPixel input) : SV_TARGET
{     
	return skyboxTexture.Sample(standardSampler, input.localPos);
}
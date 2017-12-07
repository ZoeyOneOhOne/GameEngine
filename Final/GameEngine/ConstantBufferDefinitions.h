#pragma once

#include <DirectXMath.h>

struct SimpleConstantBuffer
{
	XMFLOAT4X4 objectTransform;
	XMFLOAT4X4 viewProjection;
	XMFLOAT4X4 objectInverseTranpose;
	XMFLOAT4 objectColor;
	XMFLOAT3 lightDirection;
	float gameTime;
	XMFLOAT3 cameraPosition;
	float pad;
	XMFLOAT2 textureScale;
	//XMFLOAT3 pointLightColor;

	//Point light related values
	float pointLightRange;
	float pointLightIntensity;
	XMFLOAT3 pointLightPosition;
	float pad2;
	XMFLOAT3 pointLightAttenuation;

	//OR you could do:
	//Pointlight pointLight; //directly in the buffer
	//BUT then you have to have the padding in the point light struct,
	//and make sure whatever comes before it is padded correctly too.
};
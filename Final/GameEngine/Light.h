#pragma once

#include <DirectXMath.h>

/*
Directional lights are infiniately far away and light in one direction
	Ex. The sun
*/
class DirectionalLight
{
public:
	XMFLOAT3 direction;
	XMFLOAT4 Diffuse;
};

/*
Has position and direction
like a finite directional light
	Ex. Flashlight
*/
class SpotLight
{
public:

	XMFLOAT4 Diffuse;
};

/*
Point lights have a position
and give off light in a sphere around that position
	Ex. Lamp
*/
class PointLight
{
public:
	float range;
	float intensity;
	XMFLOAT3 position;
	XMFLOAT3 attenuation;//How a light dies off over distance
	XMFLOAT4 Diffuse;
	XMFLOAT4 Ambient;
	XMFLOAT4 Specular;
};
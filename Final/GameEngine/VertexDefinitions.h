#pragma once//include guard

#include <DirectXMath.h>//XMFLOAT3
#include <random>

#include "ModelImporter.h"

struct VertexPosTexNormTan
{
	static D3D11_INPUT_ELEMENT_DESC vertexDesc[];
	static UINT numElements;
	static UINT stride;

	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;

	VertexPosTexNormTan()
	{

	}

	VertexPosTexNormTan(XMFLOAT3 Pos, XMFLOAT2 Tex, XMFLOAT3 Norm)
		: position(Pos),
		texture(Tex),
		normal(Norm)
	{

	}

	VertexPosTexNormTan(ModelImporter::VertexObjFormat v)
	{
		position = v.Pos;
		texture = v.TextureCoords;
		normal = v.Normal;
	}
};
typedef VertexPosTexNormTan VertexStandard;

/*
Represents a vertex with Position, texture, and normal data for shading.
*/
struct VertexPosTexNorm
{
	static D3D11_INPUT_ELEMENT_DESC vertexDesc[];
	static UINT numElements;
	static UINT stride;

	XMFLOAT3 position;
	XMFLOAT2 texture;//U and V coordinates - where this 3D point is in 2D Texture Space
	XMFLOAT3 normal;

	VertexPosTexNorm()
	{

	}

	VertexPosTexNorm(XMFLOAT3 Pos, XMFLOAT2 Tex, XMFLOAT3 Norm)
		: position(Pos),
		texture(Tex),
		normal(Norm)
	{

	}

	VertexPosTexNorm(ModelImporter::VertexObjFormat v)
	{
		position = v.Pos;
		texture = v.TextureCoords;
		normal = v.Normal;
	}
};

typedef XMFLOAT3 Vector3;

struct VertexPositionColorNormal
{
	static D3D11_INPUT_ELEMENT_DESC vertexDesc[];
	static UINT numElements;
	static UINT stride;

	Vector3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;

	VertexPositionColorNormal(XMFLOAT3 pos, XMFLOAT4 col, XMFLOAT3 norm)
	{
		position = pos;
		color = col;
		normal = norm;
	}

	VertexPositionColorNormal(ModelImporter::VertexObjFormat objVertex)
	{
		position = objVertex.Pos;

		//Make the vertices light grey to white so we can still see the shape of the objects
		//float random = (float)rand() / RAND_MAX;//gives a random between 0 and 1
		//random *= .3f;//gives us a random between 0 and .3
		//random += .7f;//gives us a random between .7 and 1
		//color = XMFLOAT4(random, random, random, 1);

		color = XMFLOAT4(1, 1, 1, 1);
		normal = objVertex.Normal;
	}
};

struct VertexPositionColor
{
	static D3D11_INPUT_ELEMENT_DESC vertexDesc[];
	static UINT numElements;
	static UINT stride;

	Vector3 position;
	XMFLOAT4 color;

	VertexPositionColor(XMFLOAT3 pos, XMFLOAT4 col)
	{
		position = pos;
		color = col;
	}

	VertexPositionColor(ModelImporter::VertexObjFormat objVertex)
	{
		position = objVertex.Pos; 

		//Make the vertices light grey to white so we can still see the shape of the objects
		float random = (float)rand() / RAND_MAX;//gives a random between 0 and 1
		random *= .3f;//gives us a random between 0 and .3
		random += .7f;//gives us a random between .7 and 1

		color = XMFLOAT4(random, random, random, 1); 
	}
};
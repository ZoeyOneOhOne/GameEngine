#pragma once

#include <DirectXMath.h>

#include "ConstantBufferDefinitions.h"
#include "Mesh.h" 

using namespace std;

class GameObject
{
public:
	XMFLOAT3 position, forward, up, rotation, scale;
	XMFLOAT3 velocity;
	XMFLOAT4X4 world;//Transformation Matrix - stores position, rot
	BaseMesh* mesh; 
	XMFLOAT4 color;
	XMFLOAT2 texScale;

	GameObject()
	{
		mesh = nullptr; 
		position = XMFLOAT3(0, 0, 0);
		rotation = XMFLOAT3(0, 0, 0);
		forward = XMFLOAT3(0, 0, 1);
		up = XMFLOAT3(0, 1, 0);
		scale = XMFLOAT3(1, 1, 1);
		color = XMFLOAT4(1, 1, 1, 1);
		velocity = XMFLOAT3(0, 0, 0);
		texScale = XMFLOAT2(1, 1);
	}

	void Update()
	{
		//position.x += velocity.x;
		//position.y += velocity.y;
		//position.z += velocity.z;
		//
		//position = Utility::Add(position, velocity);

		XMStoreFloat3(&position, XMLoadFloat3(&position) + XMLoadFloat3(&velocity));
	}

	void Draw(SimpleConstantBuffer &gameCBuffer, ID3D11Buffer *gpuCBuffer, ID3D11DeviceContext *deviceContext)
	{
		//5 * 2 = 2 * 5;//Commutative 
		//Matrix multiplication is not commutative

		//Calculate new world matrix (a.k.a. Transform matrix - has position, rotation, scale of this object)
		XMStoreFloat4x4(&world, 
			XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) 
			 * XMMatrixScaling(scale.x, scale.y, scale.z) 
			* XMMatrixTranslation(position.x, position.y, position.z)
				); 

		XMMATRIX tmp = XMLoadFloat4x4(&world);

		//Store the world matrix on the constant buffer to give to the GPU
		XMStoreFloat4x4(&gameCBuffer.objectTransform,
			XMMatrixTranspose(tmp));

		//Setup Inverse Transpose matrix, for non-uniform scaling of normals.
		tmp.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1);//Manually set row 4 of this matrix to zero out its translation
		XMStoreFloat4x4(&gameCBuffer.objectInverseTranpose, XMMatrixInverse(&XMMatrixDeterminant(tmp), tmp));

		gameCBuffer.objectColor = color;
		gameCBuffer.textureScale = texScale;

		//Send the updated constant buffer to the GPU
		deviceContext->UpdateSubresource(
			gpuCBuffer,
			0,
			NULL,
			&gameCBuffer,
			0,
			0);
		 
		//Draw the object
		if (mesh)
			mesh->Draw(deviceContext);
		else
			printf("Error: Mesh not applied to GameObject.\n");
	}
};
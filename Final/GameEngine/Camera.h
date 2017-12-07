#pragma once

#include <DirectXMath.h>

#include "ConstantBufferDefinitions.h"

using namespace std;

class Camera
{
public:
	XMFLOAT3 position, forward, up, right, rotation;
	float totalPitch, maxPitch;
	XMFLOAT4X4 view, projection;

	Camera()//Constructor gets called automatically when we create an object
	{
		position = XMFLOAT3(0, 10, -25);
		rotation = XMFLOAT3(0, 0, 0);		
		forward = XMFLOAT3(0, -.25f, 1);
		right = XMFLOAT3(1, 0, 0);
		up = XMFLOAT3(0, 1, 0);
		totalPitch = 0;
		maxPitch = 89;
	}

	void MoveForward(float speed)
	{
		position.x += speed * forward.x;
		position.y += speed * forward.y;
		position.z += speed * forward.z;

		//OR
		//position = Utility::Add(position, forward);
	}

	void DepthSort(vector<GameObject> &vec)
	{
		//Sort predicates (the function that defines how we sort) are tricky if it isn't just 'ascending or descending order'
		//We can use a variety of ways to handle it, 
		//but this particular case is difficult because we need to sort by comparing 2 objects to a 3rd object, the camera
		//So we can use either a Functor (a class with an overloaded () operator) or a lambda function, like this:
		sort(vec.begin(), vec.end(), [this](GameObject &a, GameObject &b) 
		{
			//Note we only use the squares of the lengths rather than the lengths themselves
			//That's because sqrt is a relatively slow operation, and it doesn't make a difference
			//when we just want to know which distance is bigger; we'll still get the right answer
			//if sqrt(x) > sqrt(y) then x > y, so skip the sqrt step.
			float dist1 = XMVector3LengthSq(XMLoadFloat3(&a.position) - XMLoadFloat3(&position)).m128_f32[0];
			float dist2 = XMVector3LengthSq(XMLoadFloat3(&b.position) - XMLoadFloat3(&position)).m128_f32[0];
			
			return dist1 > dist2;
			
			//A Thought on Optimization
			//Firstly, only do this with partially-transparent, Alpha Blended objects. Nothing else needs it.
			//If you had a game with Lots of transparent objects, it would be more efficient to calculate the distance 
			//from the object to the camera and store that in each object in one loop,
			//then use the stored value in the sorting predicate.
			//That way it never gets calculated more than once per object.
			//The downside: Each object takes one more float worth of memory. It'd be more worth it the more transparent objects you had.
			//You could also have a TransparentGameObject child class of GameObject, so only the transparent ones have the extra 4 bytes.
		});
	}

	void Strafe(float speed)
	{
		XMStoreFloat3(&position, XMLoadFloat3(&position) + XMLoadFloat3(&right) * speed);
	}

	void Update(SimpleConstantBuffer &gameCBuffer, float aspectRatio)
	{
		if (totalPitch + rotation.x > maxPitch)
		{
			rotation.x = maxPitch - totalPitch;//Rotate the rest of the way to maxPitch
			totalPitch = maxPitch;
		}
		else if (totalPitch + rotation.x < -maxPitch)
		{
			rotation.x = -maxPitch - totalPitch;
			totalPitch = -maxPitch;
		}
		else
			totalPitch += rotation.x;

		//Load vectors, do Yaw rotation
		XMVECTOR forwardVec = XMVector3Transform(XMLoadFloat3(&forward), XMMatrixRotationY(XMConvertToRadians(rotation.y)));
		XMVECTOR rightVec = XMLoadFloat3(&right);
		rightVec = XMVector3Transform(rightVec, XMMatrixRotationY(XMConvertToRadians(rotation.y)));

		//Do pitch rotation around local right axis
		forwardVec = XMVector3Transform(forwardVec, XMMatrixRotationAxis(rightVec, -XMConvertToRadians(rotation.x)));
		 
		forwardVec = XMVector3Normalize(forwardVec);
		rightVec = XMVector3Normalize(rightVec);

		//Store results
		XMStoreFloat3(&forward, forwardVec);
		XMStoreFloat3(&right, rightVec);

		//cout << Utility::VectorToString(v) << endl;

		//Create view matrix (camera position and orientation)
		XMStoreFloat4x4(&view, XMMatrixLookAtLH(
			XMLoadFloat3(&position),							//Position of the camera, our 'eye' in the game
			XMLoadFloat3(&position) + XMLoadFloat3(&forward),	//Where the camera is looking at
			XMLoadFloat3(&up)));								//The direction that is 'up' relative to the camera.

		//Create the projection matrix (camera field of view, render distance, screen size)
		XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(
			.25f * XM_PI,	//FOV. this must be in RADIANS. That's why things were a little distorted near the edge of the screen before. My fault! Sorry :(
			aspectRatio,
			.1f,			//Near plane
			1000));			//Far plane
		
		//Multiply view and projection matrix and store in the constant buffer for shaders to use
		XMStoreFloat4x4(&gameCBuffer.viewProjection,
			XMMatrixTranspose(XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection)));
	}
};
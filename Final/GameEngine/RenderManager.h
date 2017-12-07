#pragma once

#include "VertexDefinitions.h"
#include "Mesh.h"

class RenderManager final
{
public:
	template<class T>//T is the type of constant buffer
	static void CreateConstantBuffer(ID3D11Buffer *&cBuffer, ID3D11Device *&device)
	{
		D3D11_BUFFER_DESC bufferDescription;//Has settings for creating a buffer, such as a Constant Buffer, Vertex Buffer, or Index Buffer
		
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));//Fill struct with 0's
		
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.ByteWidth = Utility::SizeAlignedTo16(sizeof(T));//Constant buffers have special memory alignment rules, they have to fit within 16 byte blocks.
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		
		HR(device->CreateBuffer(&bufferDescription, NULL, &cBuffer));//The NULL here means we aren't giving it any initial data.
	}

	template<class T>//T is type of Vertex
	static BaseMesh* CreateMesh(vector<T> vertices, vector<UINT> indices, ID3D11Device *&device)
	{
		Mesh<T>* mesh = new Mesh<T>(); 

		mesh->numVertices = vertices.size();
		mesh->numIndices = indices.size();
		CreateVertexBuffer(vertices, mesh->vertexBuffer, device);
		CreateIndexBuffer(indices, mesh->indexBuffer, device);

		return mesh;
	}

	template<class T>//T is type of Vertex, this variation of the function is just if you have arrays instead of vectors.
	static BaseMesh* CreateMesh(int numVertices, int numIndices, T vertices[], UINT indices[], ID3D11Device *&device)
	{
		Mesh<T>* mesh = new Mesh<T>();

		mesh->numVertices = numVertices;
		mesh->numIndices = numIndices;
		CreateVertexBuffer(numVertices, vertices, mesh->vertexBuffer, device);
		CreateIndexBuffer(numIndices, indices, mesh->indexBuffer, device);

		return mesh;
	}

	template<class T>//T is type of Vertex
	static void CreateVertexBuffer(int numVertices, T vertices[], ID3D11Buffer *&vBuffer, ID3D11Device *&device)
	{
		D3D11_BUFFER_DESC bufferDescription;//Has settings for creating a buffer, such as a Constant Buffer, Vertex Buffer, or Index Buffer
		
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));//Fill struct with 0's
		
		bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;//immutable is a synonym for [const]ant - standard vertex buffers can't change after being made 
		bufferDescription.ByteWidth = numVertices * sizeof(T);
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA initialData;//This stores our vertex array so it can be put on the vertex buffer for the GPU to use
		initialData.pSysMem = vertices;
		
		HR(device->CreateBuffer(&bufferDescription, &initialData, &vBuffer));
	}

	template<class T>//T is type of Vertex
	static void CreateVertexBuffer(vector<T> &vertices, ID3D11Buffer *&vBuffer, ID3D11Device *&device)
	{
		CreateVertexBuffer(vertices.size(), vertices.data(), vBuffer, device);
	}

	static void CreateIndexBuffer(int numIndices, UINT indices[], ID3D11Buffer *&iBuffer, ID3D11Device *&device)
	{
		D3D11_BUFFER_DESC bufferDescription;//Has settings for creating a buffer, such as a Constant Buffer, Vertex Buffer, or Index Buffer
		
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));//Fill struct with 0's

		bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;//immutable is a synonym for [const]ant - standard vertex buffers can't change after being made 
		bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDescription.ByteWidth = numIndices * sizeof(UINT);//Bytes in 6 UINT's - size of our index buffer
		
		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = indices;
		
		HR(device->CreateBuffer(&bufferDescription, &initialData, &iBuffer));
	}

	static void CreateIndexBuffer(vector<UINT> indices, ID3D11Buffer *&iBuffer, ID3D11Device *&device)
	{
		CreateIndexBuffer(indices.size(), indices.data(), iBuffer, device);
	}

	template<class T>//T is the type of Vertex for this Shader.
	static void CreateVertexShaderAndInputLayout(LPWSTR fileName, ID3D11VertexShader *&shader, ID3D11InputLayout *&layout, ID3D11Device *&device)
	{
		ID3DBlob* vsBlob;

		HR(D3DReadFileToBlob(fileName, &vsBlob));//The HR macro around this will check and see if the function fails and give us useful error messages if it does.

		//Creates a shader from the file read above
		HR(device->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			NULL,
			&shader));

		HR(device->CreateInputLayout(
			T::vertexDesc,
			T::numElements,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			&layout));
	}

	static void CreatePixelShader(LPWSTR fileName, ID3D11PixelShader *&shader, ID3D11Device *&device)
	{
		ID3DBlob* psBlob;//Blobs store 'blobs' or chunks of data, in this case the raw shader file. Notice we need the * on both to make both pointers.	
		HR(D3DReadFileToBlob(fileName, &psBlob));
		HR(device->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			NULL,
			&shader));
	}

	static void CreateBlendState(ID3D11BlendState *&blendState, ID3D11Device *device,
		bool enable = true,
		bool alphaToCoverage = false,
		D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA, //Default values provide Alpha Blending
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND srcAlphaBlend = D3D11_BLEND_ONE, 
		D3D11_BLEND destAlphaBlend = D3D11_BLEND_ZERO, 
		D3D11_BLEND_OP blendOP = D3D11_BLEND_OP_ADD)
	{
		D3D11_BLEND_DESC bsd; 
		ZeroMemory(&blendState, sizeof(ID3D11BlendState));
		ZeroMemory(&bsd, sizeof(D3D11_BLEND_DESC));
		bsd.AlphaToCoverageEnable = alphaToCoverage;
		bsd.IndependentBlendEnable = false;//This is if we have different render targets, we can set alpha settings differently for each. We can't use that for now.

		for (int i = 0; i < 8; ++i)
		{
			bsd.RenderTarget[i].BlendEnable = enable;
			bsd.RenderTarget[i].SrcBlend = srcBlend;
			bsd.RenderTarget[i].DestBlend = destBlend;
			bsd.RenderTarget[i].BlendOp = blendOP;
			bsd.RenderTarget[i].SrcBlendAlpha = srcAlphaBlend;
			bsd.RenderTarget[i].DestBlendAlpha = destAlphaBlend;
			bsd.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			bsd.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		device->CreateBlendState(&bsd, &blendState);
	}

	static void CreateRasterizerState(ID3D11RasterizerState *&rasterizerState, ID3D11Device *&device,
		D3D11_FILL_MODE fillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE cullMode = D3D11_CULL_MODE::D3D11_CULL_BACK)
	{
		//The Rasterizer is the stage of the rendering pipeline that interpolates between vertices.
		//It decides how to blend colors and other data across pixels, if we should draw in wireframe or not,
		//if we should use backface culling (usually we do), etc.
		//These are standard settings you can find in DX11 documentation, the book, etc.
		//You don't need to change any of them except for very specific CGI effects.
		D3D11_RASTERIZER_DESC rd;

		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));//Fill struct with 0's

		rd.AntialiasedLineEnable = true;
		rd.CullMode = cullMode;
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0.0f;
		rd.DepthClipEnable = true;
		rd.FillMode = fillMode;
		rd.FrontCounterClockwise = false;
		rd.MultisampleEnable = true;
		rd.ScissorEnable = false;
		rd.SlopeScaledDepthBias = 0.0f;

		HR(device->CreateRasterizerState(&rd, &rasterizerState));
	}

	static void CreateDepthStencilState(ID3D11DepthStencilState *&depthStencilState, ID3D11Device *&device, 
		bool enableDepth = true, D3D11_COMPARISON_FUNC comparisonMethod = D3D11_COMPARISON_LESS, 
		D3D11_DEPTH_WRITE_MASK depthWrite = D3D11_DEPTH_WRITE_MASK_ALL)
	{
		//The depth stencil state is responsible for determining the depth of pixels, 
		//making objects draw behind other objects appropriately, ignoring pixels
		//that are occluded by other objects, etc.
		//These are standard settings you can find in DX11 documentation, the book, etc.
		//You don't need to change any of them except for very specific CGI effects.
		D3D11_DEPTH_STENCIL_DESC dsd;

		ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));//Fill struct with 0's

		dsd.DepthEnable = enableDepth;
		dsd.DepthWriteMask = depthWrite;
		dsd.DepthFunc = comparisonMethod;
		dsd.StencilEnable = true;
		dsd.StencilReadMask = 0xFF;
		dsd.StencilWriteMask = 0xFF;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(device->CreateDepthStencilState(&dsd, &depthStencilState));
	}

	//ShaderResourceView = Texture
	static ID3D11ShaderResourceView* CreateTexture(LPWSTR fileName, ID3D11Device *&device, ID3D11DeviceContext *&deviceContext)
	{
		ID3D11ShaderResourceView* resource;
		std::wstring f = std::wstring(fileName);
		
		if (f.find(L".dds") != std::wstring::npos)
		{
			if (FAILED(CreateDDSTextureFromFile(device, fileName, NULL, &resource)))			
				printf("File Not Found %ls", fileName); 			
		}
		else
		{
			if (FAILED(CreateWICTextureFromFile(device, deviceContext, fileName, NULL, &resource)))
				printf("File Not Found %ls", fileName);		
		}
		return resource;
	}
	
	static ID3D11SamplerState* CreateSamplerState(ID3D11Device *&device,
		D3D11_FILTER filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC, 
		D3D11_TEXTURE_ADDRESS_MODE textureMode = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
		float minLOD = 0,
		float maxLOD = 10)
	{
		ID3D11SamplerState *samplerState;

		D3D11_SAMPLER_DESC samplerdesc;

		ZeroMemory(&samplerdesc, sizeof(D3D11_DEPTH_STENCIL_DESC));//Fill struct with 0's

		samplerdesc.Filter = filter;
		samplerdesc.AddressU = textureMode;
		samplerdesc.AddressV = textureMode;
		samplerdesc.AddressW = textureMode;
		samplerdesc.MaxAnisotropy = 1;
		samplerdesc.ComparisonFunc = D3D11_COMPARISON_LESS;
		samplerdesc.MinLOD = minLOD;
		samplerdesc.MaxLOD = maxLOD;

		device->CreateSamplerState(&samplerdesc, &samplerState);

		return samplerState;
	}
};
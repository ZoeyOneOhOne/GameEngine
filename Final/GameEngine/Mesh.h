#pragma once

//BaseMesh class will let us use Mesh's in GameObjects without having to know which Vertex Type (the template class T) they use
class BaseMesh
{
public:
	virtual void Draw(ID3D11DeviceContext* deviceContext) = 0;//=0 here makes this a pure virtual function, so BaseMesh becomes an Abstract Class.
	virtual ~BaseMesh() {}
		//This virtual destructor is IMPORTANT
		//Because our map (or whatever structure we use) stores BaseMesh pointers,
		//and not Mesh<T>'s, it won't know to use the Mesh's destructor and 
		//and we end up with memory leaks. We have to make the Base Class Destructor
		//Virtual so it uses the appropriate child-class destructor
};

template<class T>
class Mesh : public BaseMesh//Mesh inherits BaseMesh
{
public:
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	UINT numVertices, numIndices;

	void Draw(ID3D11DeviceContext* deviceContext) override//Overriding the Draw function of BaseMesh, so we use Polymorphism to draw whatever type of mesh we want.
	{
		UINT offset = 0;//Offsets are used if we have more than one separate mesh stored in one vertex buffer for optimizing memory usage
		
		//Telling the GPU what mesh to use
		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &T::stride, &offset);//IA is Input Assembler		
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//deviceContext->Draw(numVertices, 0);//arguments are number of vertices and which vertex we start at, usually 0
		deviceContext->DrawIndexed(numIndices, 0, 0);//if using index buffer
	}

	~Mesh()
	{
		SafeRelease(vertexBuffer);
		SafeRelease(indexBuffer);
	}
};
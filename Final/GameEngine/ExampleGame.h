#ifndef EXAMPLEGAME
#define EXAMPLEGAME

#include <DirectXMath.h>
#include <d3dcompiler.h> 
#include <map>

#include "BaseGame.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "ConstantBufferDefinitions.h"
#include "VertexDefinitions.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"
#include "RenderManager.h"

/*
This represents a particular game and its entities, meshes, shaders, etc.  Just remember that the Program
actually starts in main.cpp and the Game Loop is defined in BaseGame.cpp
*/
class ExampleGame final : public BaseGame
{
public:
	ExampleGame(HINSTANCE instance);
 	~ExampleGame();
	bool Init() override;
	void GetInput(float dt) override;
	void Update(float dt) override;
	void Clear() override;
	void Draw() override;
	void OnResize() override;
	void OnFocus() override;

private:
	float clearcolor[4];
	SpriteFont *garamondFont;
	SpriteBatch *spriteBatch;
	ID3D11RasterizerState* standardRasterizerState, *frontCullingRasterizerState, *wireframeRasterizerState;
	ID3D11DepthStencilState* depthStencilState, *skyDepthState, *additiveBlendDepthState;
	ID3D11BlendState* additiveBlend, *standardBlend, *noBlend, *foliageBlend;
	Camera camera;
	bool wireframe = false;

	//Constant Buffers
	SimpleConstantBuffer gameCBuffer;
	ID3D11Buffer *gpuCBuffer = nullptr;
	
	//Shaders, Input Layouts. Notice we don't have to have same number of each (we can mix them)
	ID3D11VertexShader* vShader = nullptr;
	ID3D11VertexShader* lightVShader = nullptr;
	ID3D11VertexShader* textureVShader = nullptr;
	ID3D11VertexShader* skyboxVShader = nullptr;
	ID3D11VertexShader* normalmappingVShader = nullptr;
	
	ID3D11PixelShader* pShader = nullptr;
	ID3D11PixelShader* lightPShader = nullptr;
	ID3D11PixelShader* texturePShader = nullptr;
	ID3D11PixelShader* skyboxPShader = nullptr;
	ID3D11PixelShader* transparencyPShader = nullptr;
	ID3D11PixelShader* normalmappingPShader = nullptr;
	ID3D11PixelShader* animationShader = nullptr;
	
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11InputLayout* lightInputLayout = nullptr;
	ID3D11InputLayout* textureInputLayout = nullptr;
	ID3D11InputLayout* normalmappingInputLayout = nullptr;
	
	//Lights
	DirectionalLight sunLight;
	PointLight pointLight;

	//Textures, Samplers, and Meshes
	unordered_map<string, ID3D11ShaderResourceView*> textures;//Same thing as 'hash table'
	unordered_map<string, ID3D11SamplerState*> samplers;	 
	unordered_map<string, BaseMesh*> meshes;

	//Game Objects
	vector<GameObject> gameObjects;
	vector<GameObject> texturedObjects;
	vector<GameObject> monkeys;
	vector<GameObject> normalGhosts;
	vector<GameObject> additiveGhosts;
	vector<GameObject> houseSquares;
	vector<GameObject> plants;
	vector<GameObject> items;
	vector<GameObject> doorwayObjects;
	GameObject ground;
	GameObject ground2;
	GameObject sky;
	GameObject potion1, potion2, potion3;
	GameObject Table, Sword, Apple, Meat, Vace, Doorway;

	vector<XMFLOAT3> spwanPoints;
};

#endif
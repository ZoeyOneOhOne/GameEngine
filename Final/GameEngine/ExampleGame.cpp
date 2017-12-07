#include "ExampleGame.h"
#include "VertexDefinitions.h" 
#include "ModelImporter.h"
#include "RenderManager.h"

using namespace std;

#pragma region Intialization

//The Game() constructor
ExampleGame::ExampleGame(HINSTANCE instance)
	: BaseGame(instance)
{
	windowCaption = L"ExampleGame ";

	vsync = false;
}

//Init() is for initializing the game Objects. 
bool ExampleGame::Init()
{
	if (!BaseGame::Init())
		return false;	 
	 
	//seed random so its different every game
	srand((UINT)time(0));

	//Clear to black
	clearcolor[0] = 0;
	clearcolor[1] = 0;
	clearcolor[2] = 0;
	clearcolor[3] = 1;

	mouse->InitMouseCallback(gameWindow);

	//Create Font and SpriteBatch for drawing text on the screen
	//Spritebatch/Font comes from the DirectX Tool Kit, which is Microsoft provided code
	spriteBatch = new SpriteBatch(deviceContext);
	garamondFont = new SpriteFont(device, L"..\\Assets\\Fonts\\garamond.spritefont");
	
	//Create Constant Buffer(s)
	RenderManager::CreateConstantBuffer<SimpleConstantBuffer>(gpuCBuffer, device);
	 
	//Create Shaders
	RenderManager::CreateVertexShaderAndInputLayout<VertexPositionColor>(L"VS_color.cso", vShader, inputLayout, device);
	RenderManager::CreateVertexShaderAndInputLayout<VertexPositionColorNormal>(L"VS_colornormal.cso", lightVShader, lightInputLayout, device);
	RenderManager::CreateVertexShaderAndInputLayout<VertexPositionColorNormal>(L"VS_skybox.cso", skyboxVShader, lightInputLayout, device);
	RenderManager::CreateVertexShaderAndInputLayout<VertexPosTexNorm>(L"VS_texturing.cso", textureVShader, textureInputLayout, device);
	RenderManager::CreateVertexShaderAndInputLayout<VertexPosTexNormTan>(L"VS_normalmapping.cso", normalmappingVShader, normalmappingInputLayout, device);
	
	RenderManager::CreatePixelShader(L"PS_color.cso", pShader, device);
	RenderManager::CreatePixelShader(L"PS_colornormal.cso", lightPShader, device);
	RenderManager::CreatePixelShader(L"PS_skybox.cso", skyboxPShader, device);
	RenderManager::CreatePixelShader(L"PS_texturing.cso", texturePShader, device);
	RenderManager::CreatePixelShader(L"PS_transparent.cso", transparencyPShader, device);
	RenderManager::CreatePixelShader(L"PS_normalmapping.cso", normalmappingPShader, device); 
	RenderManager::CreatePixelShader(L"PS_Animation.cso", animationShader, device);

	//Create Rasterizer State(s)
	RenderManager::CreateRasterizerState(standardRasterizerState, device);
	RenderManager::CreateRasterizerState(wireframeRasterizerState, device, D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);//Try these!
	RenderManager::CreateRasterizerState(frontCullingRasterizerState, device, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	RenderManager::CreateBlendState(standardBlend, device);
	RenderManager::CreateBlendState(foliageBlend, device, false, true);
	RenderManager::CreateBlendState(additiveBlend, device, true, false, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	RenderManager::CreateBlendState(noBlend, device, false); 

	//Create Depth Stencil State(s)
	RenderManager::CreateDepthStencilState(depthStencilState, device);
	//RenderManager::CreateDepthStencilState(skyDepthState, device, true, D3D11_COMPARISON_GREATER);//An interesting effect...
	RenderManager::CreateDepthStencilState(skyDepthState, device, true, D3D11_COMPARISON_LESS_EQUAL);
	RenderManager::CreateDepthStencilState(additiveBlendDepthState, device, true, D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);

	//Create Textures and Sampler States
	textures["RandomNoise"] = RenderManager::CreateTexture(L"../Assets/Textures/random_3.png", device, deviceContext);
	textures["Leaves"] = RenderManager::CreateTexture(L"../Assets/Textures/Leaves.png", device, deviceContext);
	//textures["Skybox"] = RenderManager::CreateTexture(L"../Assets/Textures/skybox.dds", device, deviceContext);
	textures["BarrelDiffuse"] = RenderManager::CreateTexture(L"../Assets/Textures/BarrelTex.jpg", device, deviceContext);
	textures["BarrelSpecular"] = RenderManager::CreateTexture(L"../Assets/Textures/BarrelSpecMap.jpg", device, deviceContext);
	textures["BarrelNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/BarrelNormalMap.jpg", device, deviceContext);
	textures["tileFloor"] = RenderManager::CreateTexture(L"../Assets/Textures/tileFloor.jpg", device, deviceContext);
	textures["tileFloorSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/tileFloorSpec.jpg", device, deviceContext);
	textures["tileFloorNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/tileFloorNormal.jpg", device, deviceContext);
	textures["WellDiffuse"] = RenderManager::CreateTexture(L"../Assets/Textures/WellTex.jpg", device, deviceContext);
	textures["WellSpecular"] = RenderManager::CreateTexture(L"../Assets/Textures/WellSpecMap.jpg", device, deviceContext);
	textures["WellNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/WellNormalMap.jpg", device, deviceContext);
	textures["floor"] = RenderManager::CreateTexture(L"../Assets/Textures/floor.jpg", device, deviceContext);
	textures["floorSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/floorSpec.jpg", device, deviceContext);
	textures["floorNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/floorNormal.jpg", device, deviceContext);
	textures["brick"] = RenderManager::CreateTexture(L"../Assets/Textures/brick.jpg", device, deviceContext);
	textures["brickSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/brickSpec.tif", device, deviceContext);
	textures["brickNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/brickNormal.tif", device, deviceContext);
	textures["ceramic"] = RenderManager::CreateTexture(L"../Assets/Textures/ceramic.jpg", device, deviceContext);
	textures["ceramicSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/ceramicSpec.jpg", device, deviceContext);
	textures["ceramicNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/ceramicNormal.jpg", device, deviceContext);
	textures["Marble"] = RenderManager::CreateTexture(L"../Assets/Textures/Marble.jpg", device, deviceContext);
	textures["chainlink"] = RenderManager::CreateTexture(L"../Assets/Textures/chainlink.jpg", device, deviceContext);
	textures["chainlinkNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/chainlinkNormal.jpg", device, deviceContext);
	textures["chainlinkSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/chainlinkSpec.jpg", device, deviceContext);
	textures["Water"] = RenderManager::CreateTexture(L"../Assets/Textures/Water.jpg", device, deviceContext);
	textures["Grass2"] = RenderManager::CreateTexture(L"../Assets/Textures/Grass2.png", device, deviceContext);
	textures["mushroom"] = RenderManager::CreateTexture(L"../Assets/Textures/mushroom.jpg", device, deviceContext);
	textures["shrub"] = RenderManager::CreateTexture(L"../Assets/Textures/shrub.png", device, deviceContext);
	textures["rust1"] = RenderManager::CreateTexture(L"../Assets/Textures/rust1.jpg", device, deviceContext);
	textures["rust2"] = RenderManager::CreateTexture(L"../Assets/Textures/rust2.jpg", device, deviceContext);
	textures["rust3"] = RenderManager::CreateTexture(L"../Assets/Textures/rust3.jpg", device, deviceContext);
	textures["rust4"] = RenderManager::CreateTexture(L"../Assets/Textures/rust4.jpg", device, deviceContext);
	textures["sand"] = RenderManager::CreateTexture(L"../Assets/Textures/sand.jpg", device, deviceContext);
	textures["sandSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/sandSpec.jpg", device, deviceContext);
	textures["sandNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/sandNormal.jpg", device, deviceContext);
	textures["roofInside"] = RenderManager::CreateTexture(L"../Assets/Textures/roofInside.jpg", device, deviceContext);
	textures["roofInsideSpec"] = RenderManager::CreateTexture(L"../Assets/Textures/roofInsideSpec.jpg", device, deviceContext);
	textures["roofInsideNormal"] = RenderManager::CreateTexture(L"../Assets/Textures/roofInsideNormal.jpg", device, deviceContext);
	textures["door"] = RenderManager::CreateTexture(L"../Assets/Textures/door.jpg", device, deviceContext);
	textures["brokenWindow1"] = RenderManager::CreateTexture(L"../Assets/Textures/brokenWindow1.jpg", device, deviceContext);
	textures["brokenWindow2"] = RenderManager::CreateTexture(L"../Assets/Textures/brokenWindow2.jpg", device, deviceContext);

	samplers["Point"] = RenderManager::CreateSamplerState(device, D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT);
	samplers["Anisotropic"] = RenderManager::CreateSamplerState(device);

	/////////////////////////////////////////////////////////Create meshes by hand//////////////////////////////////////////////////////////////////////////////
	vector<VertexPosTexNormTan> vertices =
	{
		{ XMFLOAT3(-2.5f, -2.5f, 0), XMFLOAT2(0, 1), XMFLOAT3(0, 0, -1) },//bottom left - 0
		{ XMFLOAT3(2.5f, 2.5f, 0),	 XMFLOAT2(1, 0), XMFLOAT3(0, 0, -1) },//top right - 1
		{ XMFLOAT3(2.5f, -2.5f, 0),	 XMFLOAT2(1, 1), XMFLOAT3(0, 0, -1) },//bottom right - 2	 
		{ XMFLOAT3(-2.5f, 2.5f, 0), XMFLOAT2(0, 0), XMFLOAT3(0, 0, -1) },//top left - 3
	};
	vector<UINT> indices = { 0, 1, 2,//Triangle 1 
		0, 3, 1 };//Triangle 2	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ModelImporter::CalcTangent(vertices, indices);
	meshes["Square"] = RenderManager::CreateMesh<VertexPosTexNormTan>(vertices, indices, device);


	//Load other models from files 
	meshes["Diamond"] = ModelImporter::LoadFromFile<VertexPositionColorNormal>("../Assets/Models/diamond.obj", device);
	meshes["Potion"] =  ModelImporter::LoadFromFile<VertexPositionColorNormal>("../Assets/Models/potion.obj", device);
	meshes["Monkey"] =  ModelImporter::LoadFromFile<VertexPositionColorNormal>("../Assets/Models/monkey.obj", device);
	meshes["Ghost"] =	ModelImporter::LoadFromFile<VertexPositionColorNormal>("../Assets/Models/Ghost2.obj", device);
	meshes["Skybox"] =  ModelImporter::LoadFromFile<VertexPositionColorNormal>("../Assets/Models/invertedcube.obj", device);
	meshes["Barrel"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/barrel.obj", device);
	meshes["Well"] =  ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/well.obj", device);
	meshes["Table"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/Table.obj", device);
	meshes["Apple"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/Apple.obj", device);
	meshes["Sword"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/Sword.obj", device);
	meshes["Vase"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/Vase.obj", device);
	meshes["Meat"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/Meat.obj", device);
	meshes["Doorway"] = ModelImporter::LoadFromFileWithTangents<VertexPosTexNormTan>("../Assets/Models/Doorway.obj", device);

	//Create game objects
	sky.mesh = meshes["Skybox"];

	//Textures objects (not all)
	texturedObjects.push_back(GameObject()); 
	texturedObjects[texturedObjects.size() - 1].mesh = meshes["Barrel"];
	texturedObjects[texturedObjects.size() - 1].position.x = -1.7f;
	texturedObjects[texturedObjects.size() - 1].position.z = 11.7f;

	texturedObjects.push_back(GameObject());
	texturedObjects[texturedObjects.size() - 1].mesh = meshes["Well"];
	texturedObjects[texturedObjects.size() - 1].position.x = 15;
	texturedObjects[texturedObjects.size() - 1].position.z = 2;

	texturedObjects.push_back(GameObject());
	texturedObjects[texturedObjects.size() - 1].mesh = meshes["Doorway"];
	texturedObjects[texturedObjects.size() - 1].position = XMFLOAT3(-2.5f, 0, 7.8f);
	texturedObjects[texturedObjects.size() - 1].scale = XMFLOAT3(0.225f, 0.125f, 0.125f);
	
	//Stuff in house
	items.push_back(GameObject());
	items[0].mesh = meshes["Table"];
	items[0].position = XMFLOAT3(0, 0.2f, 10);
	items[0].color = XMFLOAT4(1, 0, 1, 1);
	items[0].scale = XMFLOAT3(1, 1, 1);

	items.push_back(GameObject());
	items[1].mesh = meshes["Sword"];
	items[1].position = XMFLOAT3(15, 0.1f, -17);
	items[1].color = XMFLOAT4(1, 1, 0, 1);
	items[1].scale = XMFLOAT3(1, 1, 1);
	items[1].rotation.z = XM_PIDIV2;

	items.push_back(GameObject());
	items[2].mesh = meshes["Meat"];
	items[2].position = XMFLOAT3(0, 1, 10);
	items[2].color = XMFLOAT4(1, 0, 0, 1);
	items[2].scale = XMFLOAT3(1, 1, 1);
	items[2].rotation.z = XM_PI / 2;

	items.push_back(GameObject());
	items[3].mesh = meshes["Apple"];
	items[3].position = XMFLOAT3(0.3f, 1, 10.3f);
	items[3].color = XMFLOAT4(1, 0, 0, 1);
	items[3].scale = XMFLOAT3(1, 1, 1);

	items.push_back(GameObject());
	items[4].mesh = meshes["Potion"];
	items[4].position = XMFLOAT3(-0.3, 0.9f, 10.4f);
	items[4].color = XMFLOAT4(1, 1, 1, 1);
	items[4].scale = XMFLOAT3(1, 1, 1);

	ground.mesh = meshes["Square"];
	ground.position = XMFLOAT3(0, 0, 0);
	ground.scale = XMFLOAT3(30, 30, 30);
	ground.rotation.x = XM_PI/2;
	ground.texScale = XMFLOAT2(30, 30);


	ground2.mesh = meshes["Square"];
	ground2.position = XMFLOAT3(15, 0.125f, 0);
	ground2.scale = XMFLOAT3(10, 1, 30);
	ground2.rotation.x = XM_PI / 2;
	ground2.texScale = XMFLOAT2(20, 20);


	//Setup house squares
	houseSquares.push_back(GameObject());
	houseSquares[houseSquares.size() - 1].mesh = meshes["Square"]; //Squares for the house
	
	houseSquares.push_back(houseSquares[0]);
	houseSquares[0].position = XMFLOAT3(0, -1, 10);
	houseSquares[0].rotation.x = XM_PIDIV2; //floor

	houseSquares.push_back(houseSquares[1]);
	houseSquares[1].position = XMFLOAT3(-2.5f, 0.1f, 10);
	houseSquares[1].rotation.y = XM_PIDIV2; //Wall

	houseSquares.push_back(houseSquares[2]);
	houseSquares[2].position = XMFLOAT3(0, 0.1f, 12.5f);
	houseSquares[2].rotation.z = XM_PIDIV2; //Back wall

	houseSquares.push_back(houseSquares[3]);
	houseSquares[3].position = XMFLOAT3(0, 2.6f, 10);
	houseSquares[3].rotation.x = XM_PIDIV2; //RoofL

	houseSquares.push_back(houseSquares[4]);
	houseSquares[4].position = XMFLOAT3(2.5f, 0.1f, 10);
	houseSquares[4].rotation.y = XM_PIDIV2; //Wall

	houseSquares.push_back(houseSquares[5]);
	houseSquares[5].position = XMFLOAT3(40, 0.1f, 0);
	houseSquares[5].rotation.x = XM_PIDIV2; //water
	houseSquares[5].rotation.z = XM_PIDIV2; //water
	houseSquares[5].scale = XMFLOAT3(10, 1, 30);

	//Doorway Squares
	doorwayObjects.push_back(GameObject());
	doorwayObjects[doorwayObjects.size() - 1].mesh = meshes["Square"]; //Squares for the doorway

	doorwayObjects.push_back(doorwayObjects[0]);
	doorwayObjects[0].position = XMFLOAT3(0, 0, 7.7f);
	//doorwayObjects[0].rotation.y = XM_PIDIV2; //Door
	doorwayObjects[0].scale = XMFLOAT3(0.3f, 1, 0.5f);

	doorwayObjects.push_back(houseSquares[1]);
	doorwayObjects[1].position = XMFLOAT3(-1.7f, 1.2f, 7.7f);
	doorwayObjects[1].rotation.z = XM_PIDIV2; //Left brokenWindow1
	doorwayObjects[1].scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

	doorwayObjects.push_back(houseSquares[2]);
	doorwayObjects[2].position = XMFLOAT3(1.7f, 1.2f, 7.7f);
	//doorwayObjects[2].rotation.z = XM_PIDIV2; //Right brokenWindow2
	//doorwayObjects[2].rotation.x = XM_PIDIV2; //Right brokenWindow2
	doorwayObjects[2].scale = XMFLOAT3(0.2f, 0.2f, 0.2f);


	//Setup grass
	plants.push_back(GameObject());
	plants[0].mesh = meshes["Square"];

	//Move grass
	for (int i=0; i<499;i++)
	{
		plants.push_back(plants[0]);
		plants[i].position = XMFLOAT3(((float)rand() / RAND_MAX - .5f) * 2 * 50, 1, ((float)rand() / RAND_MAX - .5f) * 2 * 50);
		plants[i].position.y = 1.2f;
		plants[i].scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	}

	//sunLight.direction = XMFLOAT3(.75f, -.46f, -.45f);//Matches skybox
	sunLight.direction = XMFLOAT3(1, -1, 1);//For Demonstration
	
	
	pointLight.position = XMFLOAT3(0, 3, 9);
	pointLight.range = 25;
	pointLight.intensity = 1;
	pointLight.attenuation = XMFLOAT3(0, 1, 0);
	/////////////////////////////////////////////////////////////Adding color to lights//////////////////////////////////////////////////////////
	pointLight.Diffuse = XMFLOAT4(1, 0, 1, 1);
	


	OnResize();
	
	return true;
}

//GetInput is what is actually being called in the game loop in basegame.cpp
//before Update() and Draw().
void ExampleGame::GetInput(float dt)//dt = delta time, the time since the last frame
{
	float speed = 10;

	if (keyboard->ButtonDown(VK_SHIFT))
		speed = 50;

	if (keyboard->ButtonDown(VK_NUMPAD8))
		pointLight.position.z += speed * dt;
	if (keyboard->ButtonDown(VK_NUMPAD5))
		pointLight.position.z -= speed *dt;
	if (keyboard->ButtonDown(VK_NUMPAD6))
		pointLight.position.x += speed *dt;
	if (keyboard->ButtonDown(VK_NUMPAD4))
		pointLight.position.x -= speed *dt;
	if (keyboard->ButtonDown(VK_NUMPAD9))
		pointLight.position.y += speed *dt;
	if (keyboard->ButtonDown(VK_NUMPAD7))
		pointLight.position.y -= speed *dt;

	if (keyboard->ButtonPressed('T'))
		wireframe = !wireframe;

	if (keyboard->ButtonDown('D'))
		camera.Strafe(speed * dt);
	if (keyboard->ButtonDown('A'))
		camera.Strafe(-speed * dt);
	if (keyboard->ButtonDown('W'))
		camera.MoveForward(speed * dt);
	if (keyboard->ButtonDown('S'))
		camera.MoveForward(-speed * dt);
	if (keyboard->ButtonDown('Q'))
		camera.position.y -= speed * dt;
	if (keyboard->ButtonDown('E'))
		camera.position.y += speed * dt; 

	if (keyboard->ButtonDown('M'))
	{
		monkeys.push_back(GameObject());
		monkeys[monkeys.size() - 1].mesh = meshes["Monkey"];
		monkeys[monkeys.size() - 1].color = Utility::RandomColor();
		monkeys[monkeys.size() - 1].rotation.y = -XM_PI / 2;
		monkeys[monkeys.size() - 1].position = XMFLOAT3(-200, 1, ((float)rand() / RAND_MAX - .5f) * 2 * 100);
		monkeys[monkeys.size() - 1].velocity = XMFLOAT3(0.5f, 0, 0);
	}

	if (keyboard->ButtonDown('G'))
	{
		normalGhosts.push_back(GameObject());
		normalGhosts[normalGhosts.size() - 1].mesh = meshes["Ghost"];
		normalGhosts[normalGhosts.size() - 1].scale = XMFLOAT3(5,5,5);
		normalGhosts[normalGhosts.size() - 1].color = Utility::RandomColor();
		normalGhosts[normalGhosts.size() - 1].rotation.y = -XM_PI / 2;
		normalGhosts[normalGhosts.size() - 1].position = XMFLOAT3(((float)rand() / RAND_MAX - .5f) * 2 * 5, 0, ((float)rand() / RAND_MAX - .5f) * 2 * 5);
		//normalGhosts[normalGhosts.size() - 1].position = XMFLOAT3(0, 0, ((float)rand() / RAND_MAX - .5f) * 2 * 9);
		normalGhosts[normalGhosts.size() - 1].velocity = XMFLOAT3(0, 0.1f, 0);
	}

	camera.rotation.y = 5 * mouse->DX();
	camera.rotation.x = 5 * mouse->DY();

	//Use VK_... for keys that don't have a character
	if (keyboard->ButtonPressed(VK_ESCAPE))
		PostQuitMessage(0);//quits the game
}

//Update is where we handle all the frame by frame processing of entities,
//like updating their positions, doing any AI, Physics, etc.
void ExampleGame::Update(float dt)
{

	for (UINT i = 0; i < monkeys.size(); ++i)
	{
		monkeys[i].Update();//make monkeys move
		if (monkeys[i].position.x > 200)
			monkeys.erase(monkeys.begin() + i--);//kill off monkeys
	}

	//Same thing as with the monkeys but for ghosts
	for (UINT i = 0; i < normalGhosts.size(); ++i)
	{
		normalGhosts[i].color.w -= 0.01f;
		normalGhosts[i].Update();//make ghosts move
		if (normalGhosts[i].position.y > 10)
			normalGhosts.erase(normalGhosts.begin() + i--);//kill off ghosts
		
	}
	
	//Could rotate the light here
	gameCBuffer.lightDirection = sunLight.direction;
	XMStoreFloat3(&gameCBuffer.lightDirection, XMVector3Normalize(XMLoadFloat3(&gameCBuffer.lightDirection)));
	
	gameCBuffer.pointLightAttenuation = pointLight.attenuation;
	gameCBuffer.pointLightIntensity = pointLight.intensity;
	gameCBuffer.pointLightPosition = pointLight.position;
	gameCBuffer.pointLightRange = pointLight.range;
	

	camera.Update(gameCBuffer, AspectRatio());
	gameCBuffer.cameraPosition = camera.position;
	sky.position = camera.position;

	gameCBuffer.gameTime = timer.TotalGameTime();
	 
	deviceContext->UpdateSubresource(
		gpuCBuffer,
		0,
		NULL,
		&gameCBuffer,
		0,
		0);
}

//Clear() resets the backbuffer texture so that we are drawing to a blank screen
//It also resets some things that get messed up by spritebatch.
void ExampleGame::Clear()
{
	deviceContext->ClearRenderTargetView(renderTargetView, clearcolor);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

//Draw is where we actually render to the screen
void ExampleGame::Draw()
{
	Clear();
	
	//Apply basic stuff used by pretty much everthing
	if(wireframe)
		deviceContext->RSSetState(wireframeRasterizerState);
	else
		deviceContext->RSSetState(frontCullingRasterizerState);
	deviceContext->OMSetDepthStencilState(depthStencilState, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->VSSetConstantBuffers(0, 1, &gpuCBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &gpuCBuffer);
	deviceContext->OMSetBlendState(noBlend, 0, 0xffffff);//turn off transparency for drawing opaque things first

	//Setup for Colored/Lit objects
	deviceContext->VSSetShader(lightVShader, NULL, 0);
	deviceContext->PSSetShader(lightPShader, NULL, 0);
	deviceContext->IASetInputLayout(lightInputLayout);
	//Set the skybox now so we can use it for reflections on our color/lit objects
	//Normally, it would be a separate texture variable in the shader, so it doesn't overwrite the texture of the objects themselves.
	deviceContext->PSSetShaderResources(0, 1, &textures["Skybox"]);
	deviceContext->PSSetSamplers(0, 1, &samplers["Anisotropic"]);

	//Draw the random 10 objects we always make
	for (UINT i = 0; i < gameObjects.size(); ++i)
		gameObjects[i].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	//Draw Monkeys (press M to make Monkeys)
	for (UINT i = 0; i < monkeys.size(); ++i)
		monkeys[i].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	potion1.Draw(gameCBuffer, gpuCBuffer, deviceContext);
	potion2.Draw(gameCBuffer, gpuCBuffer, deviceContext);
	potion3.Draw(gameCBuffer, gpuCBuffer, deviceContext);


	//Draw Table stuff
	items[1].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	items[2].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	items[3].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	items[4].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	deviceContext->PSSetShaderResources(0, 1, &textures["floor"]); //floor
	deviceContext->PSSetShaderResources(1, 1, &textures["floorSpec"]);
	deviceContext->PSSetShader(normalmappingPShader, NULL, 0);
	deviceContext->PSSetShaderResources(1, 1, &textures["floorNormal"]);
	items[0].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	deviceContext->PSSetShader(texturePShader, NULL, 0);

	//Setup for Textured objects
	deviceContext->VSSetShader(textureVShader, NULL, 0);
	deviceContext->PSSetShader(texturePShader, NULL, 0);
	deviceContext->IASetInputLayout(textureInputLayout);

	
	//Drawying my doorway
	deviceContext->PSSetShaderResources(0, 1, &textures["rust1"]); //Doorway
	texturedObjects[2].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	
	//Draw Grass and mushrooms

	for (UINT i=0;i<399;i++)
	{
		if (i<=50)
		{
			deviceContext->OMSetBlendState(foliageBlend, 0, 0xffffff);
			deviceContext->PSSetShaderResources(0, 1, &textures["shrub"]);
			plants[i].Draw(gameCBuffer, gpuCBuffer, deviceContext);
		}
		else {
				deviceContext->OMSetBlendState(foliageBlend, 0, 0xffffff);
			deviceContext->PSSetShaderResources(0, 1, &textures["Grass2"]);
			plants[i].Draw(gameCBuffer, gpuCBuffer, deviceContext);
		}
	}

	//Draw doorway
	deviceContext->PSSetShaderResources(0, 1, &textures["door"]); //Door
	doorwayObjects[0].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	deviceContext->PSSetShaderResources(0, 1, &textures["brokenWindow1"]); //Left Window
	doorwayObjects[1].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	deviceContext->PSSetShaderResources(0, 1, &textures["brokenWindow2"]); //Right Window
	doorwayObjects[2].Draw(gameCBuffer, gpuCBuffer, deviceContext);


	//Draw House Squares
	deviceContext->PSSetShaderResources(0, 1, &textures["floor"]); //floor
	deviceContext->PSSetShaderResources(1, 1, &textures["floorSpec"]);
	deviceContext->PSSetShader(normalmappingPShader, NULL, 0);
	deviceContext->PSSetShaderResources(1, 1, &textures["floorNormal"]);
	houseSquares[0].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	deviceContext->PSSetShader(texturePShader, NULL, 0);

	deviceContext->PSSetShaderResources(0, 1, &textures["rust1"]); //left wall
	houseSquares[0].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	deviceContext->PSSetShaderResources(0, 1, &textures["rust1"]); //left wall
	houseSquares[1].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	deviceContext->PSSetShaderResources(0, 1, &textures["rust3"]); //back wall
	houseSquares[2].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	deviceContext->PSSetShaderResources(0, 1, &textures["roofInside"]); //roof
	deviceContext->PSSetShaderResources(1, 1, &textures["roofInsideSpec"]); //roof
	deviceContext->PSSetShader(normalmappingPShader, NULL, 0);
	deviceContext->PSSetShaderResources(2, 1, &textures["roofInsideNormal"]); //roof
	houseSquares[3].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	deviceContext->PSSetShader(texturePShader, NULL, 0);

	deviceContext->PSSetShaderResources(0, 1, &textures["rust4"]); //right
	houseSquares[4].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	deviceContext->PSSetShader(animationShader, NULL, 0);
	deviceContext->PSSetShaderResources(0, 1, &textures["Water"]);
	houseSquares[5].Draw(gameCBuffer, gpuCBuffer, deviceContext);


	//Setup for Normal-Mapped objects (also have both lights, a diffuse texture, and a specular map too).
	deviceContext->VSSetShader(normalmappingVShader, NULL, 0);
	deviceContext->PSSetShader(normalmappingPShader, NULL, 0);
	deviceContext->IASetInputLayout(normalmappingInputLayout);

	//Draw Barrel
	 deviceContext->PSSetShaderResources(0, 1, &textures["BarrelDiffuse"]);
	deviceContext->PSSetShaderResources(1, 1, &textures["BarrelSpecular"]);
	deviceContext->PSSetShaderResources(2, 1, &textures["BarrelNormal"]);
	deviceContext->PSSetSamplers(0, 1, &samplers["Anisotropic"]);
	texturedObjects[0].Draw(gameCBuffer, gpuCBuffer, deviceContext);
	

	//Draw Well
	deviceContext->PSSetShaderResources(0, 1, &textures["WellDiffuse"]);
	deviceContext->PSSetShaderResources(1, 1, &textures["WellSpecular"]);
	deviceContext->PSSetShaderResources(2, 1, &textures["WellNormal"]);
	deviceContext->PSSetSamplers(0, 1, &samplers["Anisotropic"]);
	texturedObjects[1].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	//Draw Ground
	deviceContext->PSSetShaderResources(0, 1, &textures["sand"]);
	deviceContext->PSSetShaderResources(1, 1, &textures["sandSpec"]);
	deviceContext->PSSetShader(normalmappingPShader, NULL, 0);
	deviceContext->PSSetShaderResources(2, 1, &textures["sandNormal"]);
	//ID3D11ShaderResourceView* nulltexture = nullptr;
	//deviceContext->PSSetShaderResources(1, 1, &nulltexture);//unset a texture. Notice you have to set a null texture, you can't just pass in 'null'
	ground.Draw(gameCBuffer, gpuCBuffer, deviceContext);

	//Draw ground2
	deviceContext->PSSetShaderResources(0, 1, &textures["sand"]);
	deviceContext->PSSetShaderResources(1, 1, &textures["sandSpec"]);
	deviceContext->PSSetShader(normalmappingPShader, NULL, 0);
	deviceContext->PSSetShaderResources(2, 1, &textures["sandNormal"]);
	//ID3D11ShaderResourceView* nulltexture = nullptr;
	//deviceContext->PSSetShaderResources(1, 1, &nulltexture);//unset a texture. Notice you have to set a null texture, you can't just pass in 'null'
	ground2.Draw(gameCBuffer, gpuCBuffer, deviceContext);

	//Draw Sky (Last of opaque things, still before transparent things so we can see the sky behind them).
	deviceContext->VSSetShader(skyboxVShader, NULL, 0);
	deviceContext->PSSetShader(skyboxPShader, NULL, 0);
	deviceContext->IASetInputLayout(lightInputLayout);
	deviceContext->PSSetShaderResources(0, 1, &textures["Skybox"]);
	deviceContext->PSSetSamplers(0, 1, &samplers["Anisotropic"]); 
	deviceContext->OMSetDepthStencilState(skyDepthState, 0);
	sky.Draw(gameCBuffer, gpuCBuffer, deviceContext); 

	// Draw Transparent Stuff after EVERYTHING else (need that stuff drawn to see it through the transparent stuff)
	deviceContext->VSSetShader(lightVShader, NULL, 0);
	deviceContext->PSSetShader(transparencyPShader, NULL, 0);
	deviceContext->IASetInputLayout(lightInputLayout);
	deviceContext->PSSetShaderResources(0, 1, &textures["Skybox"]);
	deviceContext->PSSetSamplers(0, 1, &samplers["Anisotropic"]);

	//Show additive blending (don't need to write to the depth buffer, order doesn't matter) 
	deviceContext->OMSetDepthStencilState(additiveBlendDepthState, 0);
	deviceContext->OMSetBlendState(additiveBlend, 0, 0xffffff);

	//Draw Ghosts (press G to make Ghosts)
	for (UINT i = 0; i < normalGhosts.size(); ++i)
		normalGhosts[i].Draw(gameCBuffer, gpuCBuffer, deviceContext);

	//Draw text on the screen (uses DXToolKit code)
	spriteBatch->Begin(); 
		garamondFont->DrawString(spriteBatch, std::wstring(L"FPS: " + std::to_wstring((int)frameRate)).c_str(), XMFLOAT2(0, 0), XMVectorSet(1,0,0,1));
	spriteBatch->End();	 

	//Finally, this actually shows the player what we drew
	HR(swapChain->Present(vsync, 0));
}	
	
//Reacquire devices when the window loses and regains focus
void ExampleGame::OnFocus()
{
	if(mouse != nullptr)
		mouse->onFocus(gameWindow);

	if(keyboard != nullptr)
		keyboard->onFocus(gameWindow);
}

//OnResize is called whenever the user changes the size of the window 
//We need to update anything that depends on the window size here.
void ExampleGame::OnResize()
{
	BaseGame::OnResize();
}

//The ~Game() destructor is for releasing and deleting all our objects
//Remember Release is for DirectX objects, and delete is for pointers that use the 'new' keyword
ExampleGame::~ExampleGame()
{
	//Release spritebatch stuff for drawing fonts
	SafeDelete(garamondFont);
	SafeDelete(spriteBatch);

	//Release rasterizer and depth stencil state
	SafeRelease(frontCullingRasterizerState);
	SafeRelease(wireframeRasterizerState);
	SafeRelease(standardRasterizerState);
	SafeRelease(depthStencilState);
	SafeRelease(skyDepthState);

	//Release shaders, layouts
	SafeRelease(vShader);
	SafeRelease(textureVShader);
	SafeRelease(lightVShader);
	SafeRelease(skyboxVShader);

	SafeRelease(pShader);
	SafeRelease(lightPShader);
	SafeRelease(texturePShader);
	SafeRelease(skyboxPShader);

	SafeRelease(inputLayout);
	SafeRelease(lightInputLayout);
	SafeRelease(textureInputLayout);

	//Release Constant Buffer
	SafeRelease(gpuCBuffer);

	for (auto i = textures.begin(); i != textures.end(); ++i)
		SafeRelease(i->second);//In a map, 'first' is the key value (a string here) and 'second' is the object being stored
	textures.clear();

	for (auto i = meshes.begin(); i != meshes.end(); ++i)
		SafeDelete(i->second);
	meshes.clear();

	for (auto i = samplers.begin(); i != samplers.end(); ++i)
		SafeRelease(i->second);
	samplers.clear();
}
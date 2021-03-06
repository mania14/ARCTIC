#include "stdafx.h"
#include "SkyBoxScene.h"
#include "../System/RenderDevice.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "../System/VertexDesc.h"
#include "../GameEngine/RenderTargetManager.h"
#include "SkyBox.h"
#include "LightObject.h"
#include "Box.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/TerrainMakerManager.h"
#include "../GameEngine/Collision.h"
#include "Terrain.h"
#include "../GameEngine/PickingManager.h"
#include "ToolTransformManager.h"
#include "../GameEngine/TerrainMakerManager.h"
#include "SceneTerrainManager.h"

SkyBoxScene::SkyBoxScene()
{
}

SkyBoxScene::~SkyBoxScene()
{
	Destroy();
}

int SkyBoxScene::Init()
{
	ToolTransformManager::This().Init();
	//TerrainMakerManager::This().Init();

	SkyBox* skybox = new SkyBox();
	skybox->Create();

	AddGameObject(OBJECT_SKYBOX, skybox);

	////for (int i = 0; i < 1; ++i)
	//{
	//	Box* box = new Box();
	//	box->Create();
	//	box->GetComponent<Transform>()->SetPosition(float3(0,0,30));
	//	//box->GetComponent<Transform>()->SetRotation(float3(270, 180, 0));
	//	//box->GetComponent<Transform>()->SetScale(float3(10, 10, 10));

	//	AddGameObject(OBJECT_STATIC, box);
	//}

	//Terrain* pTerrain = new Terrain();
	//pTerrain->Create();
	//AddGameObject(OBJECT_STATIC, pTerrain);
	
	LightObject* pLightObj = new LightObject();
	pLightObj->Create();
	AddGameObject(OBJECT_LIGHT, pLightObj);


	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, 1, 1.0f, 1000.f);
	AddGameObject(OBJECT_SKYBOX, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	RenderTargetManager::This().Init();


	//TerrainMakerManager::This().MakeHeightMap(512, 512, "Test1.png");

	return 0;
}

int SkyBoxScene::Update()
{
	InputManager::This().Update();

	BaseScene::Update();

	PickingManager::This().Update();
	ToolTransformManager::This().Update();

	return 0;
}

int SkyBoxScene::Render()
{
	//mesh render
	{
		RenderTargetDesc rtDesc;
		rtDesc._RenderTargetList.push_back(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_COLOR));
		rtDesc._RenderTargetList.push_back(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_NORMAL));
		rtDesc._RenderTargetList.push_back(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_SPECPOWER));
		rtDesc._DepthType = eRTDT_USE_DEPTH;

		RenderDevice::This().Begin(rtDesc);
		{
			BaseScene::Render();
			PickingManager::This().Render();
			ToolTransformManager::This().Render();
			//TerrainMakerManager::This().Render();
		}
		RenderDevice::This().End();
	}

	//lighting
	{
		RenderTargetDesc rtDesc;
		rtDesc._RenderTargetList.push_back(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER));
		rtDesc._DepthType = eRTDT_USE_READONLY_DEPTH;

		RenderDevice::This().Begin(rtDesc);
		{
			RenderDevice::This().SetRenderState(eRS_NO_USE_DEPTHSTENCIL);
			LightManager::This().Render();
			RenderDevice::This().SetRenderState(eRS_DEFAUT);
		}
		RenderDevice::This().End();
	}

	return 0;
}

int SkyBoxScene::Destroy()
{
	RenderTargetManager::This().Release();
	BaseScene::Destroy();
	return 0;
}

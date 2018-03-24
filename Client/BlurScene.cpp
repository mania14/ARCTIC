#include "stdafx.h"
#include "BlurScene.h"
#include "../System/RenderDevice.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "../System/VertexDesc.h"
#include "Tree.h"
#include "Terrain.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/RenderTargetManager.h"

BlurScene::BlurScene()
{
}

BlurScene::~BlurScene()
{
	Destroy();
}

int BlurScene::Init()
{
	Terrain* pTerrain = new Terrain();
	pTerrain->Create();
	XMFLOAT3 pCurPosition = pTerrain->GetComponent<Transform>()->GetPosition();
	pTerrain->GetComponent<Transform>()->SetPosition(XMFLOAT3(pCurPosition.x, pCurPosition.y + 10, pCurPosition.z));

	AddGameObject(OBJECT_STATIC, pTerrain);

	Tree* pTree = new Tree();
	pTree->Create();

	AddGameObject(OBJECT_STATIC, pTree);

	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, (float)width / (float)height, 1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	//LightManager::This().SetDirectinalLight(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(0.57735f, 0.57735f, 0.57735f));

	return 0;
}

int BlurScene::Update()
{
	InputManager::This().Update();
	//LightManager::This().Update();

	BaseScene::Update();

	return 0;
}

int BlurScene::Render()
{
	RenderDevice::This().Begin(&RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER)->m_pRenderTargetView);
	{
		BaseScene::Render();
	}
	RenderDevice::This().End(&RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER)->m_pRenderTargetView, 1);

	return 0;
}

int BlurScene::Destroy()
{
	BaseScene::Destroy();
	return 0;
}

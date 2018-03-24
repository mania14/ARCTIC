#include "stdafx.h"
#include "InstanceScene.h"
#include "../System/RenderDevice.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "../System/VertexDesc.h"
#include "Box.h"

InstanceScene::InstanceScene()
{
}

InstanceScene::~InstanceScene()
{
	Destroy();
}

int InstanceScene::Init()
{
	Box* box = new Box();
	box->Create();

	AddGameObject(OBJECT_STATIC, box);

	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, (float)width / (float)height,1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	//LightManager::This().SetDirectinalLight(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(0.57735f, 0.57735f, 0.57735f));

	return 0;
}

int InstanceScene::Update()
{
	InputManager::This().Update();
	//LightManager::This().Update();

	BaseScene::Update();

	return 0;
}

int InstanceScene::Render()
{
	BaseScene::Render();

	return 0;
}

int InstanceScene::Destroy()
{
	BaseScene::Destroy();
	return 0;
}

#include "stdafx.h"
#include "BoxScene.h"
#include "../System/RenderDevice.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "../System/VertexDesc.h"
#include "Box.h"
#include "../GameEngine/Collision.h"
#include "../GameEngine/Transform.h"

BoxScene::BoxScene()
{
}

BoxScene::~BoxScene()
{
	Destroy();
}

int BoxScene::Init()
{
	pBox1 = new Box();
	pBox1->Create();

	AddGameObject(OBJECT_STATIC, pBox1);

	pBox2 = new Box();
	pBox2->Create();

	AddGameObject(OBJECT_STATIC, pBox2);

	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, (float)width / (float)height,1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	LightManager::This().SetDirectinalLight(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(0.57735f, 0.57735f, 0.57735f));

	return 0;
}

int BoxScene::Update()
{
	static XMFLOAT3 scale(2.0f, 4.0f, 2.0f);
	static XMFLOAT3 pos(12.0f, 40.0f, 22.0f);

	pBox1->GetComponent<Transform>().SetScale(scale);
	pBox1->GetComponent<Transform>().SetPosition(pos);

	static XMFLOAT3 scale2(4.0f, 3.0f, 4.0f);
	static XMFLOAT3 pos2(22.0f, 10.0f, 10.0f);

	pBox2->GetComponent<Transform>().SetScale(scale2);
	pBox2->GetComponent<Transform>().SetPosition(pos2);

	InputManager::This().Update();
	//LightManager::This().Update();

	BaseScene::Update();

	pBox1->GetComponent<Collision>().PlaneToOBB(pBox2->GetComponent<Collision>());

	return 0;
}

int BoxScene::Render()
{
	BaseScene::Render();

	return 0;
}

int BoxScene::Destroy()
{
	BaseScene::Destroy();
	return 0;
}

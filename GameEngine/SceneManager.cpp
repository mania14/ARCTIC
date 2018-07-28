#include "stdafx.h"
#include "SceneManager.h"
//#include "PhysicsManager.h"
#include "LightManager.h"
#include "../System/GameTimer.h"

void SceneManager::InitScene(BaseScene* pScene)
{
	if(_CurrentScene)
	{
		Release();
	}

	_CurrentScene = pScene;
	_CurrentScene->Init();
}

void SceneManager::Update()
{
	float deltaTime = GameTimer::This().DeltaTime();
	_CurrentScene->Update();

	LightManager::This().Update(_CurrentScene->getObjectList(BaseScene::OBJECT_LIGHT));

	//PhysicsManager::This().AsyncUpdate(deltaTime);

	//PhysicsManager::This().WaitForAsync();
}

void SceneManager::Render()
{
	_CurrentScene->Render();
}

void SceneManager::Release()
{
	_CurrentScene->Destroy();
	//PhysicsManager::This().Release();
}

#include "stdafx.h"
#include "BaseScene.h"
#include "../System/RenderDevice.h"

BaseScene::BaseScene()
{
	
}

BaseScene::~BaseScene()
{

}

int BaseScene::Init()
{
	return 0;
}

int BaseScene::Update()
{
	for (int i = 0; i < OBJECT_COUNT; ++i)
	{
		for_each(SceneList[i].begin(), SceneList[i].end(), [](GameObject* pGameObj)->void {
			pGameObj->Update();
		});
	}

	return 0;
}

int BaseScene::Render()
{
	for (int i = 0; i < OBJECT_COUNT; ++i)
	{
		for_each(SceneList[i].begin(), SceneList[i].end(), [](GameObject* pGameObj)->void {
			pGameObj->Render();
		});
	}

	return 0;
}

int BaseScene::Destroy()
{
	for (int i = 0; i < OBJECT_COUNT; ++i)
	{
		for_each(SceneList[i].begin(), SceneList[i].end(), [](GameObject* pGameObj)->void {
			delete pGameObj;
		});

		SceneList[i].clear();
	}

	return 0;
}

int BaseScene::AddGameObject(ObjectType object_type, GameObject * pObject)
{
	SceneList[object_type].push_back(pObject);
	return 0;
}

int BaseScene::RemoveGameObject(ObjectType object_type, GameObject * pObject)
{
	SceneList[object_type].remove(pObject);
	return 0;
}

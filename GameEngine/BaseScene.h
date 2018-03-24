#pragma once
#include <list>
#include "../GameEngine/GameObject.h"

class BaseScene
{
public:
	enum ObjectType {
		OBJECT_SKYBOX,
		OBJECT_STATIC,
		OBJECT_TOOL,
		OBJECT_LIGHT,
		OBJECT_COUNT
	};
public:
	BaseScene();
	virtual ~BaseScene();

public:
	virtual int Init();
	virtual int Update();
	virtual int Render();
	virtual int Destroy();

	int AddGameObject(ObjectType object_type, GameObject* pObject);
	int RemoveGameObject(ObjectType object_type, GameObject* pObject);
	const std::list<GameObject*>& getObjectList(ObjectType object_type) const { return SceneList[object_type]; };

protected:
	std::list<GameObject*> SceneList[OBJECT_COUNT];
};
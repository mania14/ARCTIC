#pragma once
#include "../CommonUtil/Singletone.h"
#include "../GameEngine/BaseScene.h"

class BaseScene;
class SceneManager : public Singletone<SceneManager>
{
public:
	explicit SceneManager() {};
	virtual ~SceneManager() {};

public:
	void InitScene(BaseScene* pScene);
	void Update();
	void Render();
	void Release();

public:
	BaseScene* GetCurrentScene() { return _CurrentScene;  };

private:
	BaseScene* _CurrentScene;
};
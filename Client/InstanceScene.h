#pragma once
#include "../GameEngine/BaseScene.h"

class InstanceScene : public BaseScene
{
public:
	InstanceScene();
	~InstanceScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();
};
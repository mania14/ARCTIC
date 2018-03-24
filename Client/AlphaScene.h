#pragma once
#include "../GameEngine/BaseScene.h"

class AlphaScene : public BaseScene
{
public:
	AlphaScene();
	~AlphaScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();
};
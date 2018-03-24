#pragma once
#include "../GameEngine/BaseScene.h"

class TestCalcShaderScene : public BaseScene
{
public:
	TestCalcShaderScene();
	~TestCalcShaderScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();
};
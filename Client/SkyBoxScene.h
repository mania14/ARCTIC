#pragma once
#include "../GameEngine/BaseScene.h"

class Box;
class SkyBoxScene : public BaseScene
{
public:
	SkyBoxScene();
	~SkyBoxScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();
};
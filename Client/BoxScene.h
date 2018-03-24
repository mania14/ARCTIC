#pragma once
#include "../GameEngine/BaseScene.h"

class Box;
class BoxScene : public BaseScene
{
public:
	BoxScene();
	~BoxScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();

public:
	Box*		pBox1;
	Box*		pBox2;
};
#pragma once
#include "../GameEngine/BaseScene.h"

class Camera;
class Terrain;
class BlurScene : public BaseScene
{
public:
	BlurScene();
	~BlurScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();

private:
	Camera* pBlurCamera;
	Terrain* pTerrain;
};
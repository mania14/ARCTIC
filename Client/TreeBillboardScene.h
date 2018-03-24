#pragma once
#include "../GameEngine/BaseScene.h"

class Camera;
class Terrain;
class Texture;
class TreeBillboardScene : public BaseScene
{
public:
	TreeBillboardScene();
	~TreeBillboardScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();

private:
	Camera* pTreeBillboardCamera;
	Terrain* pTerrain;

	Texture* pTexture;
};
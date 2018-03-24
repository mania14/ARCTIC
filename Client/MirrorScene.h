#pragma once
#include "../GameEngine/BaseScene.h"

class Camera;
class Terrain;
class MirrorScene : public BaseScene
{
public:
	MirrorScene();
	~MirrorScene();

public:
	int Init();
	int Update();
	int Render();
	int Destroy();

private:
	void CalcMirrorCamera(Camera* _pCamera);

private:
	Camera* pMirrorCamera;
	Terrain* pTerrain;
};
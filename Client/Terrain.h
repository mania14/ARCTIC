#pragma once
#include "../GameEngine/GameObject.h"

static const int g_VoxelSize = 33;

class Texture;
class Terrain : public GameObject
{
public:
	explicit Terrain();
	virtual ~Terrain();

public:
	void Create() override;
	void SetSectorIndex(acm::int3 posIdx);
	void Update() override;
	void Render() override;
	void Destroy() override;

public:
	void SetVolumeTexture(Texture* pVolumeTexture);
};
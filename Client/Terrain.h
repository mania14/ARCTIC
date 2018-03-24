#pragma once
#include "../GameEngine/GameObject.h"

class Terrain : public GameObject
{
public:
	explicit Terrain();
	virtual ~Terrain();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;
};
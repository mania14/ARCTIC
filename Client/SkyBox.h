#pragma once
#include "../GameEngine/GameObject.h"

class SkyBox : public GameObject
{
public:
	explicit SkyBox();
	virtual ~SkyBox();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;
};
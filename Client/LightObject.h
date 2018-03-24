#pragma once
#include "../GameEngine/GameObject.h"

class LightObject : public GameObject
{
public:
	explicit LightObject();
	virtual ~LightObject();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;
};
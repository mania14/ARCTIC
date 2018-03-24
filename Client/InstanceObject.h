#pragma once
#include "../GameEngine/GameObject.h"

class InstanceObject : public GameObject
{
public:
	explicit InstanceObject();
	virtual ~InstanceObject();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;
};
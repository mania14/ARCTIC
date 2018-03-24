#pragma once
#include "../GameEngine/GameObject.h"

class Box : public GameObject
{
public:
	explicit Box();
	virtual ~Box();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;
};
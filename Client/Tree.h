#pragma once
#include "../GameEngine/GameObject.h"

class Tree : public GameObject
{
public:
	explicit Tree();
	virtual ~Tree();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;
};
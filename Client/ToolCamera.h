#pragma once
#include "../GameEngine/Camera.h"
#include "../System/InputManager.h"

class ToolCamera : public Camera
{
public:
	ToolCamera();
	~ToolCamera();

public:
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;
};
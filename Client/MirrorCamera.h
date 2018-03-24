#pragma once
#include "../GameEngine/Camera.h"

class MirrorCamera : public Camera
{
public:
	MirrorCamera();
	~MirrorCamera();

public:
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

};
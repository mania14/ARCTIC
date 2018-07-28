#include "stdafx.h"
#include "MirrorCamera.h"
#include "../GameEngine/Transform.h"
#include "../system/GameTimer.h"
#include <iostream>

MirrorCamera::MirrorCamera()
{
}

MirrorCamera::~MirrorCamera()
{
}

void MirrorCamera::Create()
{
	Camera::Create();
}

void MirrorCamera::Update()
{
	Camera::Update();

	float4x4 R = MakeReflectMatrix(float3(0, -1, 0));
	_mView = R * _mView;
	_mViewProj = _mView * _mProj;

}

void MirrorCamera::Render()
{
	Camera::Render();
}

void MirrorCamera::Destroy()
{
	Camera::Destroy();
}

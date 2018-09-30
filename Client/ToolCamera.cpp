#include "stdafx.h"
#include "ToolCamera.h"
#include "../GameEngine/Transform.h"
#include "../system/GameTimer.h"
#include <iostream>

ToolCamera::ToolCamera()
{
}

ToolCamera::~ToolCamera()
{
}

void ToolCamera::Create()
{
	Camera::Create();
}

void ToolCamera::Update()
{
	Transform* pTransform = GetComponent<Transform>();
	pTransform->SetSpeed(10);

	float3 posi = pTransform->GetPosition();
	float3 rotate = pTransform->GetRotation();

	// 카메라 이상함
	if(GetAsyncKeyState(VK_RBUTTON) & 0x8001)
	{
		// 회전 카메라 이상함
		rotate.y += InputManager::This().GetMoveMousePosX() * GameTimer::This().DeltaTime() * 50;
		rotate.x += InputManager::This().GetMoveMousePosY() * GameTimer::This().DeltaTime() * 50;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		// 회전 카메라 이상함
		rotate.x -= GameTimer::This().DeltaTime() * 100;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		// 회전 카메라 이상함
		rotate.x += GameTimer::This().DeltaTime() * 100;
	}


	if (GetAsyncKeyState(0x57) & 0x8001)
	{
		float3 look = pTransform->GetLook();
		posi += look * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x53) & 0x8001)
	{
		float3 look = pTransform->GetLook();
		posi -= look * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x41) & 0x8001)
	{
		float3 vCross = acm::float3::cross(float3(0, 1, 0), pTransform->GetLook());

		posi -= vCross * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x44) & 0x8001)
	{
		float3 vCross = acm::float3::cross(float3(0, 1, 0), pTransform->GetLook());

		posi += vCross * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x51) & 0x8001)
	{
		posi += float3(0, 1, 0) * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x45) & 0x8001)
	{
		posi -= float3(0, 1, 0) * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}


	pTransform->SetRotation(rotate);
	pTransform->SetPosition(posi);

	Camera::Update();
}

void ToolCamera::Render()
{
	Camera::Render();
}

void ToolCamera::Destroy()
{
	Camera::Destroy();
}

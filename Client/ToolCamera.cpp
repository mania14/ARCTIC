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

	XMFLOAT3 posi = pTransform->GetPosition();
	XMFLOAT3 rotate = pTransform->GetRotation();

	if(GetAsyncKeyState(VK_RBUTTON) & 0x8001)
	{
		rotate.y += InputManager::This().GetMoveMousePosX() * GameTimer::This().DeltaTime() * 100;
		rotate.x += InputManager::This().GetMoveMousePosY() * GameTimer::This().DeltaTime() * 100;

		//OutputDebugString(L"asdasdasd");
		pTransform->SetRotation(rotate);
	}

	if (GetAsyncKeyState(0x57) & 0x8001)
	{
		XMFLOAT3 look = pTransform->GetLook();
		posi.x += look.x * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.y += look.y * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.z += look.z * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x53) & 0x8001)
	{
		XMFLOAT3 look = pTransform->GetLook();
		posi.x -= look.x * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.y -= look.y * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.z -= look.z * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x41) & 0x8001)
	{
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(0, 1, 0));
		XMVECTOR vLook = XMLoadFloat3(&pTransform->GetLook());

		XMVECTOR vCross = XMVector3Cross(vUp, vLook);

		XMFLOAT3 cross;
		XMStoreFloat3(&cross, vCross);

		posi.x -= cross.x * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.y -= cross.y * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.z -= cross.z * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x44) & 0x8001)
	{
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(0, 1, 0));
		XMVECTOR vLook = XMLoadFloat3(&pTransform->GetLook());

		XMVECTOR vCross = XMVector3Cross(vUp, vLook);

		XMFLOAT3 cross;
		XMStoreFloat3(&cross, vCross);

		posi.x += cross.x * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.y += cross.y * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.z += cross.z * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x51) & 0x8001)
	{
		XMFLOAT3 vUp = pTransform->GetUp();

		posi.x += vUp.x * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.y += vUp.y * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.z += vUp.z * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

	if (GetAsyncKeyState(0x45) & 0x8001)
	{
		XMFLOAT3 vUp = pTransform->GetUp();

		posi.x -= vUp.x * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.y -= vUp.y * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
		posi.z -= vUp.z * GameTimer::This().DeltaTime() * pTransform->GetSpeed();
	}

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

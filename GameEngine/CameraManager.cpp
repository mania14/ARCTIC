#include "stdafx.h"
#include "CameraManager.h"
#include <utility>

CameraManager::CameraManager()
	:_CurrentCamera(nullptr)
{

}

CameraManager::~CameraManager()
{

}

void CameraManager::Update_CurrentCamera()
{
	_CurrentCamera->Update();
}

bool CameraManager::SetCurrentCamera(const std::string CamName)
{
	auto iter = _CameraMap.find(CamName);

	if(iter != _CameraMap.end())
	{
		_CurrentCamera = (*iter).second;

		return true;
	}else
	{
		return false;
	}
}

const XMMATRIX & CameraManager::GetCurrentCameraViewProj()
{
	return _CurrentCamera->GetViewProj();
}

const XMMATRIX & CameraManager::GetCurrentCameraProj()
{
	return _CurrentCamera->GetProj();
}

const XMMATRIX & CameraManager::GetCurrentCameraView()
{
	return _CurrentCamera->GetView();
}


void CameraManager::Release()
{

}

const XMFLOAT3 CameraManager::GetCurrentCameraPosition()
{
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, _CurrentCamera->GetPosition());
	return temp;
}

const XMFLOAT3 CameraManager::GetCurrentCameraDirection()
{
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, XMVector3Normalize(_CurrentCamera->GetAt()));
	return temp;
}
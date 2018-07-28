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

const acm::float4x4 & CameraManager::GetCurrentCameraViewProj()
{
	return _CurrentCamera->GetViewProj();
}

const acm::float4x4 & CameraManager::GetCurrentCameraProj()
{
	return _CurrentCamera->GetProj();
}

const acm::float4x4 & CameraManager::GetCurrentCameraView()
{
	return _CurrentCamera->GetView();
}


void CameraManager::Release()
{

}

const acm::float3 CameraManager::GetCurrentCameraPosition()
{
	return _CurrentCamera->GetPosition();
}

const acm::float3 CameraManager::GetCurrentCameraDirection()
{
	return _CurrentCamera->GetAt();
}
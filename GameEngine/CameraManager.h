#pragma once
#include "stdafx.h"
#include "../CommonUtil/Macro.h"
#include "../CommonUtil/Singletone.h"
#include "Camera.h"
#include <string>
#include <map>

class CameraManager : public Singletone<CameraManager>
{
public:
	explicit CameraManager();
	virtual ~CameraManager();

	template <class T>
	T*									CreateCamera(const std::string CamName, const float FovY, const float Aspect, const float Near, const float Far)
	{
		T* pCamera = new T();
		pCamera->SetFov(FovY);
		pCamera->SetAspect(Aspect);
		pCamera->SetNear(Near);
		pCamera->SetFar(Far);

		_CameraMap.insert(std::make_pair(CamName, pCamera));

		return pCamera;
	};

	void								Update_CurrentCamera();
	bool								SetCurrentCamera(const std::string CamName);
	const acm::float4x4&				GetCurrentCameraViewProj();
	const acm::float4x4&				GetCurrentCameraProj();
	const acm::float4x4&				GetCurrentCameraView();
	const acm::float3					GetCurrentCameraPosition();
	const acm::float3					GetCurrentCameraDirection();
	Camera*								GetCurrentCamera() {
		return _CurrentCamera;
	};

private:
	void								Release();

private:
	std::map<std::string, Camera*>		_CameraMap;
	Camera*								_CurrentCamera;

};
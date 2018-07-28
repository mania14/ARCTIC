#include "stdafx.h"
#include "../CommonUtil/CommonDefine.h"
#include "Camera.h"
#include "Transform.h"
#include "../System/RenderDevice.h"

Camera::Camera()
	:_FovY(0),
	_Aspect(0),
	_Near(0),
	_Far(0)
{
	Create();
}

Camera::~Camera()
{
	Destroy();
}

void Camera::Create()
{
	//Transform

	_FovY = 45.f * DEGREE_TO_RADIAN;
	_Aspect = (float)RenderDevice::This().GetWidth() / (float)RenderDevice::This().GetHeight();
	_Near = 0.1f;
	_Far = 10000.f;

	const acm::float3 vZero(0.f, 0.f, 0.f);
	_vEye = acm::float3(0.f, 0.f, -10.f);
	_vAt = acm::float3(0.f, 0.f, 0.f);
	_vUp = POSITIVE_Y;

	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);
}

void Camera::Update()
{
	GameObject::Update();

	Transform* pTransform = GetComponent<Transform>();

	_vEye = pTransform->GetPosition();
	_vAt = pTransform->GetLook();
	_vUp = pTransform->GetUp();

	_mView = MakeLookAtLH(_vEye, _vEye + _vAt, POSITIVE_Y);
	_mProj = MakePerspectiveFovLH(_FovY, _Aspect, _Near, _Far);

	_mViewProj = _mView * _mProj;
}

void Camera::Render()
{
	
}

void Camera::Destroy()
{
	GameObject::Destroy();
}

void Camera::SetCameraProjectProperty(const float Fov, const float Aspect, const float Near, const float Far)
{
	_FovY = Fov;
	_Aspect = Aspect;
	_Near = Near;
	_Far = Far;
}

void Camera::SetCameraViewProperty(const acm::float3* Eye, const acm::float3* At, const acm::float3* Up)
{
	_vEye = *Eye;
	_vAt = *At;
	_vUp = *Up;
}

const acm::float4x4 & Camera::GetView()
{
	return _mView;
}

const acm::float4x4 & Camera::GetProj()
{
	return _mProj;
}

const acm::float4x4 & Camera::GetViewProj()
{
	return _mViewProj;
}

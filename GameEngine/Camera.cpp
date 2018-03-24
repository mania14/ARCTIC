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

	const XMFLOAT3 vZero(0.f, 0.f, 0.f);
	_vEye = XMLoadFloat3(&XMFLOAT3(0.f, 0.f, -10.f));
	_vAt = XMLoadFloat3(&vZero);
	_vUp = XMLoadFloat3(&POSITIVE_Y);

	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);
}

void Camera::Update()
{
	GameObject::Update();

	Transform* pTransform = GetComponent<Transform>();

	_vEye = XMLoadFloat3(&pTransform->GetPosition());
	_vAt = XMLoadFloat3(&pTransform->GetLook());
	_vUp = XMLoadFloat3(&pTransform->GetUp());

	_mView = XMMatrixLookAtLH(_vEye, _vEye + _vAt, XMLoadFloat3(&XMFLOAT3(0, 1, 0)));
	_mProj = XMMatrixPerspectiveFovLH(_FovY, _Aspect, _Near, _Far);

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

void Camera::SetCameraViewProperty(const XMFLOAT3* Eye, const XMFLOAT3* At, const XMFLOAT3* Up)
{
	_vEye = XMLoadFloat3(Eye);
	_vAt = XMLoadFloat3(At);
	_vUp = XMLoadFloat3(Up);
}

const XMMATRIX & Camera::GetView()
{
	return _mView;
}

const XMMATRIX & Camera::GetProj()
{
	return _mProj;
}

const XMMATRIX & Camera::GetViewProj()
{
	return _mViewProj;
}

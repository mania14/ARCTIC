#pragma once
#include "GameObject.h"
#include "DirectXMath.h"


using namespace DirectX;

static const acm::float3 POSITIVE_Y = acm::float3(0, 1, 0);

class Camera : public GameObject
{
public:
	explicit Camera();
	virtual ~Camera();

public:
	void Create() override;
	void Update() override;
	void Render() override;
	void Destroy() override;

	void SetCameraProjectProperty(const float Fov, const float Aspect, const float Near, const float Far);
	void SetCameraViewProperty(const acm::float3* Eye, const acm::float3* At, const acm::float3* Up);
	void SetFov(const float Fov) { _FovY = Fov; };
	void SetAspect(const float Aspect) { _Aspect = Aspect; };
	void SetNear(const float Near) { _Near = Near; };
	void SetFar(const float Far) { _Far = Far; };

	float GetFov() { return _FovY; };
	float GetAspect() { return _Aspect; };
	float GetNear() { return _Near; };
	float GetFar() { return _Far; };

	acm::float3 GetPosition() {
		return _vEye;
	};

	acm::float3 GetAt() {
		return _vAt;
	};

	const acm::float4x4& GetView();
	const acm::float4x4& GetProj();
	const acm::float4x4& GetViewProj();

protected:
	float			_FovY;
	float			_Aspect;
	float			_Near;
	float			_Far;
	
	acm::float3		_vEye;
	acm::float3		_vAt;
	acm::float3		_vUp;

	acm::float4x4		_mView;
	acm::float4x4		_mProj;
	acm::float4x4		_mViewProj;
};
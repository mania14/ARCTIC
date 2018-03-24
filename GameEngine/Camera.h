#pragma once
#include "GameObject.h"
#include "DirectXMath.h"


using namespace DirectX;

static const XMFLOAT3 POSITIVE_Y = XMFLOAT3(0, 1, 0);

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
	void SetCameraViewProperty(const XMFLOAT3* Eye, const XMFLOAT3* At, const XMFLOAT3* Up);
	void SetFov(const float Fov) { _FovY = Fov; };
	void SetAspect(const float Aspect) { _Aspect = Aspect; };
	void SetNear(const float Near) { _Near = Near; };
	void SetFar(const float Far) { _Far = Far; };

	float GetFov() { return _FovY; };
	float GetAspect() { return _Aspect; };
	float GetNear() { return _Near; };
	float GetFar() { return _Far; };

	XMVECTOR GetPosition() {
		return _vEye;
	};

	XMVECTOR GetAt() {
		return _vAt;
	};

	const XMMATRIX& GetView();
	const XMMATRIX& GetProj();
	const XMMATRIX& GetViewProj();

protected:
	float			_FovY;
	float			_Aspect;
	float			_Near;
	float			_Far;
	
	XMVECTOR		_vEye;
	XMVECTOR		_vAt;
	XMVECTOR		_vUp;

	XMMATRIX		_mView;
	XMMATRIX		_mProj;
	XMMATRIX		_mViewProj;
};
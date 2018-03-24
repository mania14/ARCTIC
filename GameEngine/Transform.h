#include "Component.h"
#include "../System/VertexDesc.h"
#include <vector>

//simd 좀 어떻게 하기
//transform 크기 줄이기

class Transform : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = TRANSFORM_COMPONENT;

public:
	explicit Transform();
	virtual ~Transform();

public:
	int				Init();
	int				Update();
	int				Release();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	void				SetPosition(const XMFLOAT3& position);
	const XMFLOAT3&		GetPosition();
	void				SetScale(const XMFLOAT3& scale);
	const XMFLOAT3&		GetScale();
	void				SetRotation(const XMFLOAT3& _Rotation);
	const XMFLOAT3&		GetRotation();
	void				SetLook(const XMFLOAT3& look);
	const XMFLOAT3&		GetLook();
	const XMFLOAT3&		GetRight();
	const XMFLOAT3&		GetUp();
	const XMMATRIX&		GetWorldMatrix();
	const XMMATRIX&		GetWorldMatrixNoScale();
	const XMMATRIX		GetWorldMatrixInvTranspose();
	const float&		GetSpeed() { return _Speed; };
	void				SetSpeed(const float speed ) { _Speed = speed; };
	const XMFLOAT3&		GetCenter() { return _Center;  };
	const XMFLOAT3&		GetExtents() { return _Extents; };

public:
	void				RenderBox();	//너도 곧

private:
	XMFLOAT3	_Position;
	XMFLOAT3	_Scale;
	XMFLOAT3	_Rotation;
	XMFLOAT3	_Look;
	XMFLOAT3	_Up;
	XMFLOAT3	_Right;
	XMMATRIX	_mWorld;
	XMMATRIX	_mWorldNoScale;
	float		_Speed;
	
	XMFLOAT3	_Center;
	XMFLOAT3	_Extents;
};
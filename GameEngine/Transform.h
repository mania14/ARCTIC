#include "Component.h"
#include "../System/VertexDesc.h"
#include <vector>
#include "Math.h"

using namespace acm;

//simd �� ��� �ϱ� -> �׳� ���̷�Ʈ�Լ� ����
//transform ũ�� ���̱�

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
	void				SetPosition(const float3& position);
	const float3&		GetPosition();
	void				SetScale(const float3& scale);
	const float3&		GetScale();
	void				SetRotation(const float3& _Rotation);
	const float3&		GetRotation();
	void				SetLook(const float3& look);
	const float3&		GetLook();
	const float3&		GetRight();
	const float3&		GetUp();
	const float4x4&		GetWorldMatrix();
	//const float4x4		GetWorldMatrixInvTranspose();
	const float&		GetSpeed() { return _Speed; };
	void				SetSpeed(const float speed ) { _Speed = speed; };
	const float3&		GetCenter() { return _Center;  };
	const float3&		GetExtents() { return _Extents; };

public:
	void				RenderBox();	//�ʵ� ��

private:
	float3	_Position;
	float3	_Scale;
	float3	_Rotation;
	float3	_Look;
	float3	_Up;
	float3	_Right;
	float4x4	_mWorld;
	float		_Speed;
	
	float3	_Center;
	float3	_Extents;
};
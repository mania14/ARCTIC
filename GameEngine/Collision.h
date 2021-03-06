#include "Component.h"
#include "../System/RenderDevice.h"
#include "PxPhysicsAPI.h"
#include <vector>

//struct Plane
//{
//	acm::float3				m_PointList[4];
//	acm::float3				m_Normal;
//	acm::float3				m_CenterPos;
//	acm::float3				GetInvNormal() { return acm::float3(m_Normal.x * -1, m_Normal.y * -1, m_Normal.z * -1); };
//	float					GetDistance() {
//		float temp = 0.0f;
//		XMStoreFloat(&temp, XMVector3Length(XMLoadFloat3(&m_CenterPos)));
//		return temp;
//	};
//};
//
//struct OBB
//{
//	Plane					m_PlaneList[6];
//	acm::float3				m_CenterPosition;
//	acm::float3				m_Axis[3];
//};

class Transform;
class Collision : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = COLLISION_COMPONENT;

public:
	explicit Collision();
	virtual ~Collision();

public:
	virtual int				Init();
	virtual int				Release();
	void					Update(Transform* pTransform);
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	//std::vector<PxRigidActor*>	vecRigidBody;
	physx::PxRigidActor*	RigidBody;
};
#pragma once
#include "../CommonUtil/Singletone.h"
#include <list>
#include <DirectXMath.h>
#include <DirectXCollision.h>

class GameObject;
class PickingManager : public Singletone<PickingManager>
{
public:
	explicit PickingManager() {};
	virtual ~PickingManager() {};

	void	init();

public:	
	size_t		RayPick(const std::list<GameObject*>& input, const int mousePosX, const int mousePosY, std::list<GameObject*>& output);
	bool		RayPick(const int mousePosX, const int mousePosY, acm::AABB& bBox, float & dist);

	void		Update();
	void		Render();
	void		Release();

	acm::float2	GetPickMoveOrigin() { return _MovePickOrigin; };
	GameObject* GetPickGameObject() {
		return _pPickingObject;
	}

private:
	GameObject*			_pPickingObject;
	acm::float2	_MovePickOrigin;
	acm::float3	_prevPickOrigin;

	DirectX::XMVECTOR	_vecOldRay;
	bool				_isClicked;

public:
	bool test = false;
};
#include "stdafx.h"
#include "PickingManager.h"
#include "Collision.h"
#include "CollisionFactory.h"
#include "CameraManager.h"
#include "Transform.h"
#include "../System/Inputmanager.h"
#include "../System/RenderDevice.h"
#include "SceneManager.h"
#include "BaseScene.h"

using namespace acm;

void PickingManager::init()
{
}

size_t PickingManager::RayPick(const std::list<GameObject*>& input, const int mousePosX, const int mousePosY, std::list<GameObject*>& output)
{
	float4x4 invMatView = CameraManager::This().GetCurrentCameraView().inverse_val();
	float4x4 matProj = CameraManager::This().GetCurrentCameraProj();

	float vx = (float)mousePosX / (float)RenderDevice::This().GetWidth() * 2.0f - 1.f;
	vx /= matProj._11;
	float vy = (float)mousePosY / (float)RenderDevice::This().GetHeight() * -2.0f + 1.f;
	vy /= matProj._22;

	// 체크하고 갑시다!!!!
	//XMVECTOR rayOrigin = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), invMatView);
	float3 fRayOrigin(invMatView._41, invMatView._42, invMatView._43);
	//XMStoreFloat3(&fRayOrigin, rayOrigin);
	_MovePickOrigin.x = _prevPickOrigin.x - fRayOrigin.x;
	_MovePickOrigin.y = _prevPickOrigin.y - fRayOrigin.y;
	_prevPickOrigin = fRayOrigin;

	auto end = input.end();
	for (auto iter = input.begin(); iter != end; ++iter)
	{
		Transform* pTransform = (*iter)->GetComponent<Transform>();
		if(nullptr != pTransform)
		{
			float4x4 invMatWorld = pTransform->GetWorldMatrix().inverse_val();
			float4x4 invMatWorldView = invMatView * invMatWorld;
			float3 vecRayDir = acm::TransformNormal(float3(vx, vy, 1.f), invMatView * invMatWorldView);
			vecRayDir.normalize();
			float3 vecRayOrigin(invMatWorldView._41, invMatWorldView._42, invMatWorldView._43);

			float dist = 0;
			BoundingBox box(pTransform->GetCenter(), pTransform->GetExtents());
			/*if(box.Intersects(vecRayOrigin, vecRayDir, dist))
			{
				output.push_back(*iter);
			}*/
		}
	}

	float3 camPos = CameraManager::This().GetCurrentCameraPosition();
	
	auto miniter = std::min_element(output.begin(), output.end(), [&](GameObject* &lhs, GameObject* &rhs)->bool
	{
		return (lhs->GetComponent<Transform>()->GetPosition() - camPos).lengthsq() > (rhs->GetComponent<Transform>()->GetPosition() - camPos).lengthsq();
	});

	if(miniter != output.end())
	{
		_pPickingObject = (*miniter);
	}else
	{
		_pPickingObject = nullptr;
	}

	return output.size();
}

bool PickingManager::RayPick(const int mousePosX, const int mousePosY, acm::AABB& bBox, float & dist)
{
	acm::float4x4 invMatView = CameraManager::This().GetCurrentCameraView().inverse_val();
	acm::float4x4 matProj = CameraManager::This().GetCurrentCameraProj();

	float vx = (float)mousePosX / (float)RenderDevice::This().GetWidth() * 2.0f - 1.f;
	vx /= matProj(0, 0);
	float vy = (float)mousePosY / (float)RenderDevice::This().GetHeight() * -2.0f + 1.f;
	vy /= matProj(1, 1);

	float3 vecRayDir = acm::TransformCoord(float3(vx, vy, 1.f), invMatView);
	vecRayDir.normalize();
	float3 vecRayOrigin = acm::TransformCoord(float3(0.0f, 0.0f, 0.0f), invMatView);

	_MovePickOrigin.x = _prevPickOrigin.x - vecRayOrigin.x;
	_MovePickOrigin.y = _prevPickOrigin.y - vecRayOrigin.y;
	_prevPickOrigin = vecRayOrigin;

	return bBox.Intersects(vecRayOrigin, vecRayDir, dist);
}

void PickingManager::Update()
{
	//if(GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	//{
	//	BaseScene* pScene = SceneManager::This().GetCurrentScene();
	//	if(nullptr != pScene)
	//	{
	//		std::list<GameObject*> rayList;
	//		RayPick(pScene->getObjectList(BaseScene::OBJECT_STATIC), InputManager::This().GetMousePosX(), InputManager::This().GetMousePosY(), rayList);

	//		if(nullptr != _pPickingObject)
	//		{
	//			// z 축

	//			BoundingBox box;
	//			float dist;
	//			float lineLength = 0.5f + 0.1f;
	//			float extents = 0.1f;

	//			Transform* pTransform = _pPickingObject->GetComponent<Transform>();
	//			box.Center = pTransform->GetPosition();
	//			box.Center.x += pTransform->GetLook().x * lineLength;
	//			box.Center.y += pTransform->GetLook().y * lineLength;
	//			box.Center.z += pTransform->GetLook().z * lineLength;
	//			box.Extents = acm::float3(extents, extents, extents);

	//			if(RayPick(InputManager::This().GetMousePosX(), InputManager::This().GetMousePosY(), box, dist))
	//			{
	//				test = true;
	//			}

	//			// x 축
	//			box.Center = pTransform->GetPosition();
	//			box.Center.x += pTransform->GetRight().x * lineLength;
	//			box.Center.y += pTransform->GetRight().y * lineLength;
	//			box.Center.z += pTransform->GetRight().z * lineLength;

	//			if (RayPick(InputManager::This().GetMousePosX(), InputManager::This().GetMousePosY(), box, dist))
	//			{
	//				test = true;
	//			}

	//			// y 축
	//			box.Center = pTransform->GetPosition();
	//			box.Center.x += pTransform->GetUp().x * lineLength;
	//			box.Center.y += pTransform->GetUp().y * lineLength;
	//			box.Center.z += pTransform->GetUp().z * lineLength;

	//			if (RayPick(InputManager::This().GetMousePosX(), InputManager::This().GetMousePosY(), box, dist))
	//			{
	//				test = true;
	//			}

	//			acm::float4x4 invMatViewProj = acm::float4x4Inverse(&acm::float4x4Determinant(CameraManager::This().GetCurrentCameraViewProj()), CameraManager::This().GetCurrentCameraViewProj());
	//			
	//			float vx = (float)InputManager::This().GetMousePosX() / (float)RenderDevice::This().GetWidth() * 2.0f - 1.f;
	//			float vy = (float)InputManager::This().GetMousePosY() / (float)RenderDevice::This().GetHeight() * -2.0f + 1.f;
	//		

	//			//XMVECTOR vecNewRay = XMVector3TransformNormal(XMVectorSet(vx, vy, newvz, 0.0f), invMatView) + XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), invMatView);
	//			XMVECTOR tposition = XMLoadFloat3(&pTransform->GetPosition());
	//			tposition = XMVector3TransformCoord(tposition, CameraManager::This().GetCurrentCameraViewProj());
	//			acm::float4 newvz;
	//			XMStoreFloat4(&newvz, tposition);

	//			XMVECTOR vecNewRay = XMVector3TransformCoord(XMVectorSet(vx, vy, newvz.z, 1.0f), invMatViewProj);
	//			float MoveDist1 = 0;

	//			acm::float3 debug;
	//			XMStoreFloat3(&debug, vecNewRay);

	//			XMStoreFloat(&MoveDist1, XMVector3Dot(vecNewRay - _vecOldRay, XMLoadFloat3(&pTransform->GetRight())));

	//			if (_isClicked)
	//			{
	//				if (test)
	//				{
	//					if(abs(MoveDist1) > 0.f)
	//					{
	//						char test[100] = "";
	//						acm::float3 tttt;
	//						XMStoreFloat3(&tttt, vecNewRay - _vecOldRay);
	//						sprintf_s(test, "test = %.2f, %.2f, %.2f\n", tttt.x, tttt.y, tttt.z);
	//						ATLTRACE(test);
	//					}
	//					acm::float3 position = pTransform->GetPosition();
	//					position.x += MoveDist1;
	//					pTransform->SetPosition(position);
	//				}
	//			}else
	//			{
	//				_isClicked = true;
	//			}
	//			_vecOldRay = vecNewRay;
	//		}
	//	}
	//}else
	//{
	//	test = false;
	//	_isClicked = false;
	//}
}

void PickingManager::Render()
{
	
}

void PickingManager::Release()
{

}
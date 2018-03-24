#include "stdafx.h"
#include "MirrorScene.h"
#include "../System/RenderDevice.h"
#include "../System/RenderState.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "MirrorCamera.h"
#include "../System/VertexDesc.h"
#include "Box.h"
#include "Terrain.h"
#include "../GameEngine/Transform.h"

MirrorScene::MirrorScene()
{
}

MirrorScene::~MirrorScene()
{
	Destroy();
}

int MirrorScene::Init()
{
	pTerrain = new Terrain();
	pTerrain->Create();
	XMFLOAT3 pCurPosition = pTerrain->GetComponent<Transform>()->GetPosition();
	pTerrain->GetComponent<Transform>()->SetPosition(XMFLOAT3(pCurPosition.x, pCurPosition.y + 10, pCurPosition.z));

	//AddGameObject(pTerrain);

	for (int i = 0; i < 10; ++i)
	{
		Box* box = new Box();
		box->Create();
		box->GetComponent<Transform>()->SetPosition(XMFLOAT3(i * 10, 20, i * 10));

		AddGameObject(OBJECT_STATIC, box);
	}

	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, (float) width / (float)height, 1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	pMirrorCamera = CameraManager::This().CreateCamera<MirrorCamera>("MirrorCamera", 0.25f * 3.14f, (float)width / (float)height, 1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pMirrorCamera);

	LightManager::This().SetDirectinalLight(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(0.57735f, 0.57735f, 0.57735f));

	return 0;
}

//void MirrorScene::CalcMirrorCamera(Camera* _pCamera)
//{
//	// 반사 카메라의 Eye, At
//
//	XMFLOAT3 pEye = _pCamera->GetComponent<Transform>()->GetPosition();
//	XMFLOAT3 pAt = _pCamera->GetComponent<Transform>()->GetLook();
//	
//	XMVECTOR MirrorNormal = XMVectorSet(0.f, 1.f, 0.f, 0.f);
//	XMVECTOR MirrorPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
//
//	XMVECTOR vEye = XMLoadFloat3(&pEye);
//	XMVECTOR MirrorDotEye = XMVector3Dot(vEye, MirrorNormal) * -MirrorNormal;
//
//	XMFLOAT3 newEye, newAt;
//	XMStoreFloat3(&newEye, vEye + (2 * MirrorDotEye));
//	XMStoreFloat3(&newAt, XMVector3Normalize(XMVector3Reflect(XMLoadFloat3(&pAt), MirrorNormal)));
//
//	pMirrorCamera->GetComponent<Transform>()->SetPosition(newEye);
//	pMirrorCamera->GetComponent<Transform>()->SetLook(newAt);
//}

void MirrorScene::CalcMirrorCamera(Camera* _pCamera)
{
	// 반사 카메라의 Eye, At

	XMFLOAT3 pEye = _pCamera->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 pAt = _pCamera->GetComponent<Transform>()->GetLook();

	pMirrorCamera->GetComponent<Transform>()->SetPosition(pEye);
	pMirrorCamera->GetComponent<Transform>()->SetLook(pAt);
}

int MirrorScene::Update()
{
	InputManager::This().Update();
	//LightManager::This().Update();

	{
		Camera* pMainCamera = CameraManager::This().GetCurrentCamera();
		CalcMirrorCamera(pMainCamera);
	}

	BaseScene::Update();

	return 0;
}

int MirrorScene::Render()
{
	RenderDevice::This().Begin();
	{
		BaseScene::Render();
		//MirrorRender
		{
			//stencil render

			RenderDevice::This().GetContext()->OMSetBlendState(RenderStates::This().NoRenderTargetWritesBS, blendFactor, 0xffffffff);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().MarkMirrorDSS, 1);

			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

			pTerrain->Update();
			pTerrain->Render();

			RenderDevice::This().GetContext()->RSSetState(0);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(0, 0);
			RenderDevice::This().GetContext()->OMSetBlendState(0, blendFactor, 0xffffffff);
		}

		{
			CameraManager::This().SetCurrentCamera("MirrorCamera");
			RenderDevice::This().GetContext()->RSSetState(RenderStates::This().NoCullRS);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().DrawReflectionDSS, 1);
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

			BaseScene::Update();
			BaseScene::Render();
			CameraManager::This().SetCurrentCamera("MainCamera");

			RenderDevice::This().GetContext()->RSSetState(0);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(0, 0);
		}

		static bool test1 = true;

		if(test1)
		{
			RenderDevice::This().GetContext()->OMSetBlendState(RenderStates::This().TransparentBS, blendFactor, 0xffffffff);
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			pTerrain->Update();
			pTerrain->Render();

			RenderDevice::This().GetContext()->RSSetState(0);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(0, 0);
		}
	}
	RenderDevice::This().End();

	return 0;
}

int MirrorScene::Destroy()
{
	BaseScene::Destroy();
	return 0;
}

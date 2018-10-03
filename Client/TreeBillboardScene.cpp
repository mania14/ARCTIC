#include "stdafx.h"
#include "TreeBillboardScene.h"
#include "../System/RenderDevice.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "../System/VertexDesc.h"
#include "Tree.h"
#include "Terrain.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/RenderTargetManager.h"
#include "../GameEngine/TextureManager.h"
#include "../System/RenderState.h"

TreeBillboardScene::TreeBillboardScene()
{
}

TreeBillboardScene::~TreeBillboardScene()
{
	Destroy();
}

int TreeBillboardScene::Init()
{
	RenderTargetManager::This().Init();

	Terrain* pTerrain = new Terrain();
	pTerrain->Create();
	XMFLOAT3 pCurPosition = pTerrain->GetComponent<Transform>()->GetPosition();
	pTerrain->GetComponent<Transform>()->SetPosition(XMFLOAT3(pCurPosition.x, pCurPosition.y + 10, pCurPosition.z));

	AddGameObject(OBJECT_STATIC, pTerrain);

	Tree* pTree = new Tree();
	pTree->Create();

	AddGameObject(OBJECT_STATIC, pTree);

	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, (float)width / (float)height, 1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	//LightManager::This().SetDirectinalLight(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(0.57735f, 0.57735f, 0.57735f));

	pTexture = TextureManager::This().LoadTexture("../Res/Texture/Tree0.dds");

	return 0;
}

int TreeBillboardScene::Update()
{
	InputManager::This().Update();
	//LightManager::This().Update();

	BaseScene::Update();

	return 0;
}

int TreeBillboardScene::Render()
{
	
	RenderDevice::This().Begin(&RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER_TEMP)->m_pRenderTargetView, 1);
	{
		BaseScene::Render();
	}
	RenderDevice::This().End(&RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER)->m_pRenderTargetView, 1);

	RenderDevice::This().Begin(&RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER)->m_pRenderTargetView, 1);
	{
		RenderDevice::This().BeginFX("HorizonBlur", 0);
		{
			RenderDevice::This().GetRawVariableByName("gInput")->AsShaderResource()->SetResource(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER_TEMP)->m_pResourceView);
			RenderDevice::This().GetRawVariableByName("gOutput")->AsUnorderedAccessView()->SetUnorderedAccessView(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BLUR)->m_pUnorderdView);
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			UINT numGroupsX = (UINT)ceilf(RenderDevice::This().GetWidth() / 256.0f);
			RenderDevice::This().GetContext()->Dispatch(numGroupsX, RenderDevice::This().GetHeight(), 1);

			// Unbind the input texture from the CS for good housekeeping.
			ID3D11ShaderResourceView* nullSRV[1] = { 0 };
			RenderDevice::This().GetContext()->CSSetShaderResources(0, 1, nullSRV);
			ID3D11UnorderedAccessView* nullSUV[1] = { 0 };
			RenderDevice::This().GetContext()->CSSetUnorderedAccessViews(0, 1, nullSUV, 0);
		}
		RenderDevice::This().EndFX();

		RenderDevice::This().BeginFX("VerticalBlur", 0);
		{
			RenderDevice::This().GetRawVariableByName("gInput")->AsShaderResource()->SetResource(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BLUR)->m_pResourceView);
			RenderDevice::This().GetRawVariableByName("gOutput")->AsUnorderedAccessView()->SetUnorderedAccessView(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER_TEMP)->m_pUnorderdView);
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			UINT numGroupsY = (UINT)ceilf(RenderDevice::This().GetHeight() / 256.0f);
			RenderDevice::This().GetContext()->Dispatch(RenderDevice::This().GetWidth(), numGroupsY, 1);

			// Unbind the input texture from the CS for good housekeeping.
			ID3D11ShaderResourceView* nullSRV[1] = { 0 };
			RenderDevice::This().GetContext()->CSSetShaderResources(0, 1, nullSRV);
			ID3D11UnorderedAccessView* nullSUV[1] = { 0 };
			RenderDevice::This().GetContext()->CSSetUnorderedAccessViews(0, 1, nullSUV, 0);
		}
		RenderDevice::This().EndFX();

		RenderDevice::This().GetContext()->CSSetShader(0, 0, 0);

		//RenderTargetManager::This().DrawScreenQuad(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BLUR));
	}
	RenderDevice::This().End(&RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_BACKBUFFER)->m_pRenderTargetView, 1);

	RenderDevice::This().GetSwapChain()->Present(0, 0);

	return 0;
}

int TreeBillboardScene::Destroy()
{
	BaseScene::Destroy();
	return 0;
}

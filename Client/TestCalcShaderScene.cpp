#include "stdafx.h"
#include "TestCalcShaderScene.h"
#include "../System/RenderDevice.h"
#include "../System/GameTimer.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/LightManager.h"
#include "ToolCamera.h"
#include "../System/VertexDesc.h"
#include "../GameEngine/Texture.h"
#include "../GameEngine/TextureManager.h"

struct ClacData
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};


TestCalcShaderScene::TestCalcShaderScene()
{
}

TestCalcShaderScene::~TestCalcShaderScene()
{
	Destroy();
}

int TestCalcShaderScene::Init()
{
	UINT width = RenderDevice::This().GetWidth();
	UINT height = RenderDevice::This().GetHeight();
	Camera* pCamera = CameraManager::This().CreateCamera<ToolCamera>("MainCamera", 0.25f * 3.14f, (float)width / (float)height, 1.0f, 1000.f);
	AddGameObject(OBJECT_STATIC, pCamera);

	CameraManager::This().SetCurrentCamera("MainCamera");

	LightManager::This().SetDirectinalLight(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(0.57735f, 0.57735f, 0.57735f));

	return 0;
}

int TestCalcShaderScene::Update()
{
	InputManager::This().Update();
	//LightManager::This().Update();

	BaseScene::Update();

	//D3D11_BUFFER_DESC outputDesc; //<- 클라이언트에서는 참조 못하게 만들어야된다.

	std::vector<ClacData> dataA(10);
	std::vector<ClacData> dataB(10);
	std::vector<ClacData> dataC(10);

	for (int i = 0; i < 10; ++i)
	{
		dataA[i].v1 = XMFLOAT3(i, i, i);
		dataA[i].v2 = XMFLOAT2(i, 0);

		dataB[i].v1 = XMFLOAT3(-i, i, 0);
		dataB[i].v2 = XMFLOAT2(i, 0);
	}

	Texture* pTextureA = TextureManager::This().CreateInputBufferTexture<ClacData>(&dataA[0], 10);
	Texture* pTextureB = TextureManager::This().CreateInputBufferTexture<ClacData>(&dataB[0], 10);
	Texture* pTextureC = TextureManager::This().CreateOutputBufferTexture<ClacData>(&dataC[0], 10);

	RenderDevice::This().Begin("testcalc");
	{
		HRESULT hr;
		hr = RenderDevice::This().GetRawVariableByName("gInputA")->AsShaderResource()->SetResource(pTextureA->m_pResourceView);
		hr = RenderDevice::This().GetRawVariableByName("gInputB")->AsShaderResource()->SetResource(pTextureB->m_pResourceView);
		hr = RenderDevice::This().GetRawVariableByName("gOutput")->AsUnorderedAccessView()->SetUnorderedAccessView(pTextureC->m_pUnorderdView);

		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

		RenderDevice::This().GetContext()->Dispatch(1, 1, 1);

		TextureManager::This().ReadBufferTexture<ClacData>(pTextureC, &dataC[0], 10);
	}

	//끝까지 돌리면안됨 수정할거많음~

	RenderDevice::This().End();

	return 0;
}

int TestCalcShaderScene::Render()
{
	BaseScene::Render();

	return 0;
}

int TestCalcShaderScene::Destroy()
{
	BaseScene::Destroy();
	return 0;
}

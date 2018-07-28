#include "stdafx.h"
#include "LightManager.h"
#include "../System/RenderDevice.h"
#include "RenderTargetManager.h"
#include "GameObject.h"
#include "CameraManager.h"

void LightManager::Update(const std::list<GameObject*>& lightObjectList)
{
	for (int i = 0; i < Light::LIGHT_COUNT; ++i)
	{
		_LightList[i].clear();
	}

	auto end = lightObjectList.end();
	for (auto iter = lightObjectList.begin(); iter != end; ++iter)
	{
		Light* pLight = (*iter)->GetComponent<Light>();
		_LightList[pLight->GetLightType()].push_back(pLight);
	}
}

void LightManager::SetDirectinalLightConstBuffer()
{
	auto end = _LightList[Light::DIRECTINAL_LIGHT].end();
	for (auto iter = _LightList[Light::DIRECTINAL_LIGHT].begin(); iter != end;++iter)
	{
		RenderDevice::This().GetRawVariableByName("gDirLight")->SetRawValue(&((*iter)->getLightBuffer()), 0, sizeof(CBufferLight));
	}
}

void LightManager::Render()
{
	RenderDevice::This().Begin("LightDefaultTech", 0);

	SetDirectinalLightConstBuffer();

	RenderDevice::This().GetRawVariableByName("gDepthTex")->AsShaderResource()->SetResource(RenderDevice::This().GetDepthStencilResourceView());
	RenderDevice::This().GetRawVariableByName("gColorTex")->AsShaderResource()->SetResource(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_COLOR)->m_pResourceView);
	RenderDevice::This().GetRawVariableByName("gNormalTex")->AsShaderResource()->SetResource(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_NORMAL)->m_pResourceView);
	RenderDevice::This().GetRawVariableByName("gSpecTex")->AsShaderResource()->SetResource(RenderTargetManager::This().GetRenderTarget(RenderTargetManager::eRT_SPECPOWER)->m_pResourceView);

	acm::float4 PerspectiveValues;
	//const acm::float4x4 pProj = CameraManager::This().GetCurrentCamera()->GetProj();
	//acm::float3 temp;

	/*XMStoreFloat3(&temp, pProj.r[0]); PerspectiveValues.x = 1.0f / temp.x;
	XMStoreFloat3(&temp, pProj.r[1]); PerspectiveValues.y = 1.0f / temp.y;
	XMStoreFloat3(&temp, pProj.r[3]); PerspectiveValues.z = temp.z;
	XMStoreFloat3(&temp, pProj.r[2]); PerspectiveValues.w = -temp.z;*/
	PerspectiveValues.x = 1.0f / CameraManager::This().GetCurrentCamera()->GetProj()._11;
	PerspectiveValues.y = 1.0f / CameraManager::This().GetCurrentCamera()->GetProj()._22;
	PerspectiveValues.z = CameraManager::This().GetCurrentCamera()->GetProj()._33;
	PerspectiveValues.w = -CameraManager::This().GetCurrentCamera()->GetProj()._33;
	RenderDevice::This().GetRawVariableByName("PerspectiveValues")->AsVector()->SetRawValue(&PerspectiveValues, 0, sizeof(acm::float4));

	acm::float4x4 invView = CameraManager::This().GetCurrentCamera()->GetView().inverse_val();
	RenderDevice::This().GetVariableByName("ViewInv")->SetMatrix(reinterpret_cast<float*>(&invView));

	RenderDevice::This().GetContext()->IASetInputLayout(NULL);
	RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	RenderDevice::This().GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, 0);
	RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
	RenderDevice::This().GetContext()->Draw(4, 0);

	RenderDevice::This().GetRawVariableByName("gDepthTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetRawVariableByName("gColorTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetRawVariableByName("gNormalTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetRawVariableByName("gSpecTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

	RenderDevice::This().End("LightDefaultTech");
}
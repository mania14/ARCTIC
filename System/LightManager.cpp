#include "stdafx.h"
#include "LightManager.h"
#include "../System/RenderDevice.h"
#include "../CameraManager.h"

void LightManager::Update()
{
	RenderDevice::GetInstance()->GetVariableByName("gDirLight")->SetRawValue(&m_GlobalDirectLight, 0, sizeof(m_GlobalDirectLight));
	//RenderDevice::GetInstance()->GetVariableByName("gPointLight")->SetRawValue(&m_GlobalDirectLight, 0, sizeof(m_GlobalDirectLight));
	ID3DX11EffectVectorVariable* pEyePosW = RenderDevice::GetInstance()->GetVariableByName("gEyePosW")->AsVector();
	pEyePosW->SetRawValue(&CameraManager::GetInstance()->GetCurrentCameraPosition(), 0, sizeof(XMVECTOR));
}

void LightManager::SetDirectinalLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, XMFLOAT3 dir)
{
	m_GlobalDirectLight._ambient = ambient;
	m_GlobalDirectLight._diffuse = diffuse;
	m_GlobalDirectLight._specular = specular;
	m_GlobalDirectLight._direction = dir;
}

void LightManager::AddDirectinalLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, XMFLOAT3 position, XMFLOAT3 attenuation, float range)
{
	PointLight newLight;
	newLight._ambient = ambient;
	newLight._diffuse = diffuse;
	newLight._specular = specular;
	newLight._position = position;
	newLight._attenuation = attenuation;
	newLight._range = range;


	m_vecPointLight.push_back(newLight);
}

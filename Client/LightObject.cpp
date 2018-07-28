#include "stdafx.h"
#include "LightObject.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/Light.h"
#include "../System/RenderDevice.h"

LightObject::LightObject()
{
}

LightObject::~LightObject()
{
	Destroy();
}

void LightObject::Create()
{
	//Transform
	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);

	//Light
	Light* pLight = new Light();
	pLight->SetLightType(Light::DIRECTINAL_LIGHT);
	pLight->SetAmbient(float4(0.0f, 0.0f, 0.0f, 0.0f));
	pLight->SetDiffuse(float4(1.0f, 1.0f, 1.0f, 1.0f));
	pLight->SetSpecular(float4(0.5f, 0.5f, 0.5f, 1.0f));
	pLight->SetDirection(float3(0.f, 0.f, 1.f));
	AddComponent(pLight);
}

void LightObject::Update()
{
	GameObject::Update();
}

void LightObject::Render()
{
}

void LightObject::Destroy()
{
	GameObject::Destroy();
}
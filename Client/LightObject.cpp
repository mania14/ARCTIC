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
	pLight->SetAmbient(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	pLight->SetDiffuse(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	pLight->SetSpecular(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	pLight->SetDirection(XMFLOAT3(0.f, 0.f, 1.f));
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
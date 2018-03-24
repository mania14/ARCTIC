#include "stdafx.h"
#include "../System/RenderDevice.h"
#include "Collision.h"
#include "directxcolors.h"
#include "Transform.h"

using namespace DirectX;

Collision::Collision()
{
}

Collision::~Collision()
{
	Release();
}

int Collision::Init()
{
	return false;
}

int Collision::Release()
{

	return true;
}

void Collision::Update(Transform * pTransform)
{
	
}

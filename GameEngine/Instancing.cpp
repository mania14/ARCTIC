#include "stdafx.h"
#include "../System/RenderDevice.h"
#include "Instancing.h"
#include "directxcolors.h"

using namespace DirectX;

Instancing::Instancing()
{
}

Instancing::~Instancing()
{
}

int Instancing::Init()
{
	return false;
}

int Instancing::Release()
{
	if (nullptr != vBuffer)
	{
		vBuffer->Release();
	}

	return true;
}
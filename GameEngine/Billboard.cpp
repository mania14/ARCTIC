#include "stdafx.h"
#include "../System/RenderDevice.h"
#include "Billboard.h"
#include "directxcolors.h"

using namespace DirectX;

Billboard::Billboard()
{
}

Billboard::~Billboard()
{
	Release();
}

int Billboard::Init()
{
	return false;
}

int Billboard::Release()
{
	if (nullptr != vBuffer)
	{
		vBuffer->Release();
	}

	return true;
}
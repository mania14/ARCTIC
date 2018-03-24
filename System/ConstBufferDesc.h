#pragma once
#include <windows.h>
#include "DirectXMath.h"

using namespace DirectX;

struct CBufferLight
{
	CBufferLight() { ZeroMemory(this, sizeof(this)); };
	XMFLOAT4 _ambient;
	XMFLOAT4 _diffuse;
	XMFLOAT4 _specular;
	XMFLOAT3 _direction;
	float	 _range;
	XMFLOAT3 _attenuation;
	float	 _spot;
	XMFLOAT3 _position;
	float	 _dummy;
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;
};
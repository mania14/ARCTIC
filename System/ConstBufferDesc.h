#pragma once
#include <windows.h>

struct CBufferLight
{
	CBufferLight() { ZeroMemory(this, sizeof(CBufferLight)); };
	acm::float4 _ambient;
	acm::float4 _diffuse;
	acm::float4 _specular;
	acm::float3 _direction;
	float	    _range;
	acm::float3 _attenuation;
	float	    _spot;
	acm::float3 _position;
	float	    _dummy;
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(Material)); }

	acm::float4 Ambient;
	acm::float4 Diffuse;
	acm::float4 Specular; // w = SpecPower
	acm::float4 Reflect;
};
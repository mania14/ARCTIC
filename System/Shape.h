#pragma once
#include "stdafx.h"

class BOX
{
public:
	BOX(acm::float3 min, acm::float3 max) {};
	~BOX() {};

public:
	acm::float3 min;
	acm::float3 max;
};
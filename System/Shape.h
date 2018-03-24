#pragma once
#include "stdafx.h"

class BOX
{
public:
	BOX(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max) {};
	~Box() {};

public:
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};
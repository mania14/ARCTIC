#pragma once
#include "Object.h"
#include "vector"

enum COMPONENTIDTYPE
{
	BILLBOARD_COMPONENT,
	MESH_COMPONENT,
	TRANSFORM_COMPONENT,
	CONTROLLER_COMPONENT,
	INSTANCING_COMPONENT,
	COLLISION_COMPONENT,
	TERRAIN_COMPONENT,
	LIGHT_COMPONENT,
	END_COMPONENT
};

class Component
{
public:
	explicit Component();
	virtual ~Component();
	virtual int	Init() { return -1; };
	virtual int	Release() { return -1; };

public:
	virtual int						Update() { return -1; };
	virtual COMPONENTIDTYPE			GetComponentID() {
		return END_COMPONENT;
	};
	static int						_refCount[END_COMPONENT];
};
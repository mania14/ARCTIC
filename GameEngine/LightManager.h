#pragma once
#include "stdafx.h"
#include "../CommonUtil/Singletone.h"
#include "Light.h"
#include <vector>

class GameObject;
class LightManager : public Singletone<LightManager>
{
public:
	void						Update(const std::list<GameObject*>& lightObjectList);
	void						SetDirectinalLightConstBuffer();
	void						Render();

private:
	std::vector<Light*>			_LightList[Light::LIGHT_COUNT];
};
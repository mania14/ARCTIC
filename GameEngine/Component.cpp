#include "stdafx.h"
#include "Component.h"

int Component::_refCount[END_COMPONENT] = {0, 0, 0, 0, 0, 0};

Component::Component()
{
}

Component::~Component()
{
}
#include "stdafx.h"
#include "../System/RenderDevice.h"
#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

int Controller::Init()
{
	
	return true;
}

int Controller::Release()
{

	return true;
}

WPARAM Controller::GetCurrentKey()
{
	return CurrentKeyInput;
}

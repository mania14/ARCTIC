#include "stdafx.h"
#include "InputManager.h"
#include "RenderDevice.h"

InputManager::InputManager()
{
	_MoveMousePos.x = 0;
	_MoveMousePos.y = 0;

	_LastMousePos.x = 0;
	_LastMousePos.y = 0;

	_CurrentMousePos.x = 0;
	_CurrentMousePos.y = 0;

	_KeyInput = 0;
}

InputManager::~InputManager()
{

}

void InputManager::Update()
{
	MouseUpdate();
}

void InputManager::MouseUpdate()
{
	::GetCursorPos(&_CurrentMousePos);
	::ScreenToClient(RenderDevice::This().GetHandle(), &_CurrentMousePos);

	_MoveMousePos.x = _CurrentMousePos.x - _LastMousePos.x;
	_MoveMousePos.y = _CurrentMousePos.y - _LastMousePos.y;

	_LastMousePos = _CurrentMousePos;

	RECT rc;
	GetClientRect(RenderDevice::This().GetHandle(), &rc);
}

void InputManager::KeyUpdate(WPARAM wParam)
{
	_KeyInput = wParam;
}

void InputManager::Release()
{
	
}
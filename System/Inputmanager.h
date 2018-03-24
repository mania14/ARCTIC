#pragma once
#include "stdafx.h"
#include "../CommonUtil/Macro.h"
#include "../CommonUtil/Singletone.h"
#include <string>
#include <map>

// 문자키 정의하기
// up, press, once 구현하기

class InputManager : public Singletone<InputManager>
{
public:
	explicit InputManager();
	virtual ~InputManager();

public:
	void			Update();
	void			MouseUpdate();
	void			KeyUpdate(WPARAM wParam);
	void			Release();

	WPARAM			GetMouseInput() { return _MouseInput; };
	WPARAM			GetKeyInput() { return _KeyInput; };
	int				GetMoveMousePosX() { return _MoveMousePos.x; };
	int				GetMoveMousePosY() { return _MoveMousePos.y; };
	int				GetMousePosX() { return _CurrentMousePos.x; };
	int				GetMousePosY() { return _CurrentMousePos.y; };

private:
	POINT			_CurrentMousePos;
	POINT			_LastMousePos;
	POINT			_MoveMousePos;
	WPARAM			_KeyInput;
	WPARAM			_MouseInput;

};
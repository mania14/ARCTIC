#include "Component.h"
#include <vector>

class Controller : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = CONTROLLER_COMPONENT;

public:
	explicit Controller();
	virtual ~Controller();

public:
	int				Init();
	int				Release();
	WPARAM			GetCurrentKey();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	

private:
	WPARAM			CurrentKeyInput;
};
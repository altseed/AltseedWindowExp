#pragma once

#include "ap.Base.h"

namespace ap
{

class Keyboard
{
public:
	Keyboard() {}
	virtual ~Keyboard() {}

	virtual InputState GetKeyState(Keys key) const { return InputState::Free; }

	static Keyboard* Create(Window* window);
};

}
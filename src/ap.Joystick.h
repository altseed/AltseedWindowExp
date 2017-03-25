#pragma once

#include "ap.Base.h"

namespace ap
{

class Joystick
{
public:
	Joystick() {}
	virtual ~Joystick() {}

	virtual InputState GetKeyState(Keys key) const { return InputState::Free; }

	static Joystick* Create(Window* window);
};

}
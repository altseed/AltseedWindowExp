#pragma once

#include "ap.Base.h"

namespace ap
{

class Mouse
{
public:
	Mouse() {}
	virtual ~Mouse() {}

	virtual InputState GetMouseButtonState(MouseButtons button) const { return InputState::Free; }

	static Mouse* Create(Window* window);
};

}
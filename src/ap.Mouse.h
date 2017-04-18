#pragma once

#include "ap.Base.h"

namespace ap
{

class Mouse
{
public:
	Mouse() {}
	virtual ~Mouse() {}

	virtual void RefreshInputState() {}

	virtual void GetPosition(float& x, float& y) const {}

	virtual void SetPosition(float x, float y) {}

	virtual float GetWheel() const { return 0; }

	virtual InputState GetMouseButtonState(MouseButtons button) const { return InputState::Free; }

	static Mouse* Create(Window* window);
};

}
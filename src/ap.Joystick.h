#pragma once

#include "ap.Base.h"

namespace ap
{

class Joystick
{
public:
	Joystick() {}
	virtual ~Joystick() {}

	virtual void RefreshInputState() {}

	virtual void RefreshConnectedState() {}

	virtual bool IsPresent(int32_t joystickIndex) { return false; }

	virtual InputState GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const { return InputState::Free; }

	virtual InputState GetButtonState(int32_t joystickIndex, JoystickButtonType type) const { return InputState::Free; }

	virtual float GetAxisState(int32_t joystickIndex, int32_t axisIndex) const { return 0; }

	virtual float GetAxisState(int32_t joystickIndex, JoystickAxisType type) const { return 0; }

	virtual JoystickType GetJoystickType(int32_t joystickIndex) const { return JoystickType::Other; }

	virtual int32_t GetButtonCount(int32_t joystickIndex) const { return 0; }

	virtual int32_t GetAxisCount(int32_t joystickIndex) const { return 0; }

	static Joystick* Create(Window* window);
};

}
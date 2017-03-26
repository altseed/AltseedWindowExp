
#include "ap.Joystick_Impl_PC.h"

namespace ap
{

Joystick_Impl_PC::Joystick_Impl_PC(Window* window)
{
	isPresent.fill(false);
	buttonCount.fill(0);
	axisCount.fill(0);

	for (int32_t i = 0; i < MAX_JOYSTICKS_NUM; i++)
	{
		currentHit[i].fill(false);
		preHit[i].fill(false);
		currentAxis[i].fill(0);
	}

	for (int i = 0; i < MAX_JOYSTICKS_NUM; i++)
	{
		isPresent[i] = glfwJoystickPresent(i) == GLFW_TRUE;
	}
}

Joystick_Impl_PC::~Joystick_Impl_PC()
{

}

void Joystick_Impl_PC::RefreshInputState()
{
	preHit = currentHit;

	for (int jind = 0; jind < MAX_JOYSTICKS_NUM; jind++)
	{
		int32_t axesCount = 0;
		auto ax = glfwGetJoystickAxes(jind, &axesCount);
		for (int i = 0; i < axesCount; ++i)
		{
			currentAxis[jind][i] = ax[i];
		}

		int32_t buttonsCount = 0;
		auto btns = glfwGetJoystickButtons(jind, &buttonsCount);

		for (int i = 0; i < buttonsCount; ++i)
		{
			currentHit[jind][i] = (bool)btns[i];
		}

		buttonCount[jind] = buttonsCount;
		axisCount[jind] = axesCount;
	}
}


bool Joystick_Impl_PC::IsPresent(int32_t joystickIndex)
{
	if (joystickIndex < 0) return false;
	if (joystickIndex >= MAX_JOYSTICKS_NUM) return false;

	return isPresent[joystickIndex];
}

InputState Joystick_Impl_PC::GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const
{
	if (currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return InputState::Hold;
	else if (!currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return InputState::Release;
	else if (currentHit[joystickIndex][buttonIndex] && !preHit[joystickIndex][buttonIndex]) return InputState::Push;
	else return InputState::Free;
}

float Joystick_Impl_PC::GetAxisState(int32_t joystickIndex, int32_t axisIndex) const
{
	return currentAxis[joystickIndex][axisIndex];
}

}
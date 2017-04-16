
#include "ap.Joystick_Impl_PC.h"

#include <string.h>

namespace ap
{

Joystick_Impl_PC::Joystick_Impl_PC(Window* window)
{
	isPresent.fill(false);
	types.fill(JoystickType::PSVITA);
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

		if (!isPresent[i]) continue;

		// Recognize joystick
		auto name = glfwGetJoystickName(i);

		int32_t buttonsCount = 0;
		auto btns = glfwGetJoystickButtons(i, &buttonsCount);

		if (strcmp(name, "Wireless Controller") == 0 && buttonsCount == 18)
		{
			types[i] = JoystickType::PS4;
		}
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
	if (buttonIndex < 0) return InputState::Free;

	if (currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return InputState::Hold;
	else if (!currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return InputState::Release;
	else if (currentHit[joystickIndex][buttonIndex] && !preHit[joystickIndex][buttonIndex]) return InputState::Push;
	else return InputState::Free;
}

InputState Joystick_Impl_PC::GetButtonState(int32_t joystickIndex, JoystickButtonType type) const
{
	auto jtype = GetJoystickType(joystickIndex);
	if (jtype == JoystickType::PSVITA) return InputState::Free;

	if (jtype == JoystickType::PS4)
	{
		std::array<int, (int32_t)JoystickButtonType::Max> maps;
		maps.fill(-1);

		maps[(int32_t)JoystickButtonType::Square] = 0;
		maps[(int32_t)JoystickButtonType::Cross] = 1;
		maps[(int32_t)JoystickButtonType::Circle] = 2;
		maps[(int32_t)JoystickButtonType::Triangle] = 3;

		maps[(int32_t)JoystickButtonType::L1] = 4;
		maps[(int32_t)JoystickButtonType::R1] = 5;
		maps[(int32_t)JoystickButtonType::L2] = 6;
		maps[(int32_t)JoystickButtonType::R2] = 7;

		maps[(int32_t)JoystickButtonType::Up] = 14;
		maps[(int32_t)JoystickButtonType::Right] = 15;
		maps[(int32_t)JoystickButtonType::Down] = 16;
		maps[(int32_t)JoystickButtonType::Left] = 17;

		return GetButtonState(joystickIndex, maps[(int32_t)type]);
	}

	return InputState::Free;
}

float Joystick_Impl_PC::GetAxisState(int32_t joystickIndex, int32_t axisIndex) const
{
	if (axisIndex < 0) return 0;

	return currentAxis[joystickIndex][axisIndex];
}

float Joystick_Impl_PC::GetAxisState(int32_t joystickIndex, JoystickAxisType type) const
{
	auto jtype = GetJoystickType(joystickIndex);
	if (jtype == JoystickType::PSVITA) return 0;

	if (jtype == JoystickType::PS4)
	{
		std::array<int, (int32_t)JoystickAxisType::Max> maps;
		maps.fill(-1);

		maps[(int32_t)JoystickAxisType::LeftH] = 0;
		maps[(int32_t)JoystickAxisType::LeftV] = 1;
		maps[(int32_t)JoystickAxisType::RightH] = 2;
		maps[(int32_t)JoystickAxisType::RightV] = 5;

		maps[(int32_t)JoystickAxisType::L2] = 3;
		maps[(int32_t)JoystickAxisType::R2] = 4;

		return GetAxisState(joystickIndex, maps[(int32_t)type]);
	}

	return 0.0f;
}

JoystickType Joystick_Impl_PC::GetJoystickType(int32_t joystickIndex) const
{
	return types[joystickIndex];
}

int32_t Joystick_Impl_PC::GetButtonCount(int32_t joystickIndex) const
{
	return buttonCount[joystickIndex];
}

int32_t Joystick_Impl_PC::GetAxisCount(int32_t joystickIndex) const
{
	return axisCount[joystickIndex];
}

}
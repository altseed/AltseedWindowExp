#pragma once

#include "../ap.Joystick.h"

namespace ap
{

	class Joystick_Impl_PC
		: public Joystick
	{
	private:
		static const int MAX_AXES_NUM = 10;
		static const int MAX_BUTTONS_NUM = 30;
		static const int MAX_JOYSTICKS_NUM = 16;

		Window*		window = nullptr;

		std::array<bool, MAX_JOYSTICKS_NUM>			isPresent;

		std::array<int32_t, MAX_JOYSTICKS_NUM>			buttonCount;
		std::array<int32_t, MAX_JOYSTICKS_NUM>			axisCount;

		std::array<std::array<bool,MAX_BUTTONS_NUM>, MAX_JOYSTICKS_NUM>	currentHit;
		std::array<std::array<bool,MAX_BUTTONS_NUM>, MAX_JOYSTICKS_NUM>	preHit;

		std::array<std::array<bool, MAX_AXES_NUM>, MAX_JOYSTICKS_NUM>	currentAxis;

	public:
		Joystick_Impl_PC(Window* window);
		virtual ~Joystick_Impl_PC();

		void RefreshInputState();

		bool IsPresent(int32_t joystickIndex);

		InputState GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const;

		float GetAxisState(int32_t joystickIndex, int32_t axisIndex) const;
	};

}
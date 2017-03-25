#pragma once

#include "../ap.Joystick.h"

namespace ap
{

	class Joystick_Impl_PC
		: public Joystick
	{
	private:
		Window*		window = nullptr;

	public:
		Joystick_Impl_PC(Window* window);
		virtual ~Joystick_Impl_PC();
	};

}
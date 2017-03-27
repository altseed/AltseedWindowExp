#include "ap.Joystick.h"

#if defined(_OTHER)
#include "Other/ap.Joystick_Impl_Other.h"
#elif defined(__ANDROID__)

#else
#include "PC/ap.Joystick_Impl_PC.h"
#endif

namespace ap
{
	Joystick* Joystick::Create(Window* window)
	{
#if defined(_OTHER)
		return new Joystick_Impl_Other(window);
#elif defined(__ANDROID__)
		return new Joystick();
#else
		return new Joystick_Impl_PC(window);
#endif
	}
}
#include "ap.Joystick.h"

#ifdef __ANDROID__
//#include "Android/ap.Joystick_Impl_Android.h"
#else
#include "PC/ap.Joystick_Impl_PC.h"
#endif

namespace ap
{
	Joystick* Create(Window* window)
	{
#ifdef __ANDROID__
		return new Joystick();
#else
		return new Joystick_Impl_PC(window);
#endif
	}
}
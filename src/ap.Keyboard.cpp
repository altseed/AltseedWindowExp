#include "ap.Keyboard.h"

#ifdef __ANDROID__
#include "Android/ap.Keyboard_Impl_Android.h"
#else
#include "PC/ap.Keyboard_Impl_PC.h"
#endif

namespace ap
{
	Keyboard* Create(Window* window)
	{
#ifdef __ANDROID__
		return new Keyboard();
#else
		return new Keyboard_Impl_PC(window);
#endif
	}
}
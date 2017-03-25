#include "ap.Mouse.h"

#ifdef __ANDROID__
//#include "Android/ap.Mouse_Impl_Android.h"
#else
#include "PC/ap.Mouse_Impl_PC.h"
#endif

namespace ap
{
	Mouse* Create(Window* window)
	{
#ifdef __ANDROID__
		return new Mouse();
#else
		return new Mouse_Impl_PC(window);
#endif
	}
}
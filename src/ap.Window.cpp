#include "ap.Window.h"

#ifdef __ANDROID__
#include "Android/ap.Window_Impl_Android.h"
#else
#include "PC/ap.Window_Impl_PC.h"
#endif

namespace ap
{
	Window* Window::Create()
	{
#ifdef __ANDROID__
		return new Window_Impl_Android();
#else
		return new Window_Impl_PC();
#endif
	}
}
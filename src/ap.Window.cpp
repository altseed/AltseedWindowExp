#include "ap.Window.h"

#if defined(_OTHER)
#include "Other/ap.Window_Impl_Other.h"
#elif defined(__ANDROID__)
#include "Android/ap.Window_Impl_Android.h"
#else
#include "PC/ap.Window_Impl_PC.h"
#endif

namespace ap
{
	Window* Window::Create()
	{
#if defined(_OTHER)
		return new Window_Impl_Other();
#elif defined(__ANDROID__)
		return new Window_Impl_Android();
#else
		return new Window_Impl_PC();
#endif
	}
}
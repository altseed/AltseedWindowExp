#include "ap.Touch.h"

#if defined(_OTHER)

#elif defined(__ANDROID__)
#include "Android/ap.Touch_Impl_Android.h"
#else
#include "PC/ap.Touch_Impl_PC.h"
#endif

namespace ap
{
	Touch* Touch::Create(Window* window)
	{
#if defined(_OTHER)
		return new Touch();
#elif defined(__ANDROID__)
		return new Touch_Impl_Android(window);
#else
		return new Touch_Impl_PC(window);
#endif
	}
}
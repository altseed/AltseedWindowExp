
#pragma once 

#include "../ap.Window.h"

namespace ap
{

	class Window_Impl_Android
		: public Window
	{
	private:

	public:
		Window_Impl_Android();
		virtual ~Window_Impl_Android();

		ErrorCode Initialize(int32_t windowSizeX, int32_t windowSizeY) override;

		bool DoEvent() override { return true; }
	};

}
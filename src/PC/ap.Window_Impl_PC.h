
#pragma once 

#include "../ap.Window.h"

namespace ap
{

	class Window_Impl_PC
		: public Window
	{
	private:
		GLFWwindow*		window = nullptr;

	public:
		Window_Impl_PC();
		virtual ~Window_Impl_PC();

		ErrorCode Initialize(int32_t windowSizeX, int32_t windowSizeY) override;

		bool DoEvent() override { return true; }
	};

}

#pragma once 

#include "../ap.Window.h"

namespace ap
{

	class Window_Impl_PC
		: public Window
	{
	private:
		GLFWwindow*		window = nullptr;

		void Terminate();

	public:
		Window_Impl_PC();
		virtual ~Window_Impl_PC();

		ErrorCode Initialize(const WindowInitializationParameter& parameter) override;

		bool DoEvent() override;
	};

}
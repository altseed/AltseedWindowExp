
#pragma once 

#include "../ap.Window.h"

namespace ap
{

	class Window_Impl_PC
		: public Window
	{
	private:
		GLFWwindow*		window = nullptr;
		bool			isOpenGLMode = true;

		void Terminate();

	public:
		Window_Impl_PC();
		virtual ~Window_Impl_PC();

		ErrorCode Initialize(const WindowInitializationParameter& parameter) override;

		bool DoEvent() override;

		void Close() override;

		void* GetHandle() const override;

		void Present() override;

		void MakeContextCurrent() override;

		void MakeContextNone() override;

		GLFWwindow* GetNativeWindow() const { return window; }
	};

}
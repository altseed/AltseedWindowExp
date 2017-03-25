
#pragma once 
#include "from_sample.h"
#include "ap.Window.h"

namespace ap
{
	class Window_Impl_Android
		: public Window
	{
	private:
		struct android_app* state;
		struct engine engine;
	public:
		Window_Impl_Android();
		virtual ~Window_Impl_Android();

		ErrorCode Initialize(const WindowInitializationParameter& parameter) override;

		bool DoEvent() override;
		void Update();

	};

}
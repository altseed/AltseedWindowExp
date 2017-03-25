
#pragma once 
#include "from_sample.h"
#include "ap.Window.h"

namespace ap
{
	void draw(struct ap::engine* engine);

	class Window_Impl_Android
		: public Window
	{
	private:
		
	public:
		Window_Impl_Android();
		virtual ~Window_Impl_Android();

		ErrorCode Initialize(const WindowInitializationParameter& parameter) override;

		bool DoEvent() override;
		void Update();

		struct android_app* state;
		struct engine engine;
	};

}
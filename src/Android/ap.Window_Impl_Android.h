
#pragma once 
#include "from_sample.h"
#include "../ap.Window.h"

namespace ap
{

	static int engine_init_display(struct engine* engine);
	static void engine_draw_frame(struct engine* engine);
	static void engine_term_display(struct engine* engine);
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
	static void engine_handle_cmd(struct android_app* app, int32_t cmd);

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
		void Update();

		bool DoEvent() override;
	};

}
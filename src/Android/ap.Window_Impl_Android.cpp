#include "from_sample.h"
#include "ap.Window_Impl_Android.h"

namespace ap
{
	Window_Impl_Android::Window_Impl_Android()
	{

	}

	Window_Impl_Android::~Window_Impl_Android()
	{

	}

	ErrorCode Window_Impl_Android::Initialize(const WindowInitializationParameter& parameter)
	{
		state =(struct android_app*) parameter.UserData[0];

		memset(&engine, 0, sizeof(engine));
		state->userData = &engine;
		state->onAppCmd = engine_handle_cmd;
		state->onInputEvent = engine_handle_input;
		engine.app = state;

		if (state->savedState != NULL) {
			// 以前の保存状態で開始します。復元してください。
			engine.state = *(struct saved_state*)state->savedState;
		}


		return ErrorCode::OK;
	}

	bool Window_Impl_Android::DoEvent()
	{
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident = ALooper_pollAll(0, NULL, &events,
			(void**)&source)) >= 0) {

			// このイベントを処理します。
			if (source != NULL) {
				source->process(state, source);
			}


			// 終了するかどうか確認します。
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return false;
			}
		}
		return true;
	}

	void Window_Impl_Android::Update()
	{
		if (engine.display == NULL) {
			// ディスプレイがありません。
			// エラー処理がいる？
			return;
		}
	}
}
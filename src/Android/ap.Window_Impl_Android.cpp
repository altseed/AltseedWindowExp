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

		// 加速度計の監視の準備
		engine.sensorManager = ASensorManager_getInstance();
		engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
			ASENSOR_TYPE_ACCELEROMETER);
		engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
			state->looper, LOOPER_ID_USER, NULL, NULL);

		if (state->savedState != NULL) {
			// 以前の保存状態で開始します。復元してください。
			engine.state = *(struct saved_state*)state->savedState;
		}

		engine.animating = 1;

		return ErrorCode::OK;
	}

	bool Window_Impl_Android::DoEvent()
	{
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// このイベントを処理します。
			if (source != NULL) {
				source->process(state, source);
			}

			// センサーにデータがある場合、今すぐ処理します。
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						LOGI("accelerometer: x=%f y=%f z=%f",
							event.acceleration.x, event.acceleration.y,
							event.acceleration.z);
					}
				}
			}

			// 終了するかどうか確認します。
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return true;
			}
		}
		return true; // ここに来たらアウト？？
	}

	void Window_Impl_Android::Update()
	{
		if (engine.animating) {
			// イベントが完了したら次のアニメーション フレームを描画します。
			engine.state.angle += .01f;
			if (engine.state.angle > 1) {
				engine.state.angle = 0;
			}

			// 描画は画面の更新レートに合わせて調整されているため、
			// ここで時間調整をする必要はありません。
			engine_draw_frame(&engine);
		}
	}
}
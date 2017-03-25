#include "from_sample.h"


namespace ap
{
	static int engine_init_display(struct ap::engine* engine) {
		// OpenGL ES と EGL の初期化

		/*
		* 目的の構成の属性をここで指定します。
		* 以下で、オンスクリーン ウィンドウと
		* 互換性のある、各色最低 8 ビットのコンポーネントの EGLConfig を選択します
		*/
		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};
		EGLint w, h, format;
		EGLint numConfigs;
		EGLConfig config;
		EGLSurface surface;
		EGLContext context;

		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(display, 0, 0);

		/* ここで、アプリケーションは目的の構成を選択します。このサンプルでは、
		* 抽出条件と一致する最初の EGLConfig を
		* 選択する単純な選択プロセスがあります */
		eglChooseConfig(display, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID は、ANativeWindow_setBuffersGeometry() に
		* よって受け取られることが保証されている EGLConfig の属性です。
		* EGLConfig を選択したらすぐに、ANativeWindow バッファーを一致させるために
		* EGL_NATIVE_VISUAL_ID を使用して安全に再構成できます。*/
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

		surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
		context = eglCreateContext(display, config, NULL, NULL);

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
			LOGW("Unable to eglMakeCurrent");
			return -1;
		}

		eglQuerySurface(display, surface, EGL_WIDTH, &w);
		eglQuerySurface(display, surface, EGL_HEIGHT, &h);

		engine->display = display;
		engine->context = context;
		engine->surface = surface;
		engine->width = w;
		engine->height = h;
		engine->state.angle = 0;

		// GL の状態を初期化します。
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		glEnable(GL_CULL_FACE);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_DEPTH_TEST);

		return 0;
	}

	/**
	* ディスプレイ内の現在のフレームのみ。
	*/
	static void engine_draw_frame(struct ap::engine* engine) {
		if (engine->display == NULL) {
			// ディスプレイがありません。
			return;
		}

		// 色で画面を塗りつぶします。
		glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
			((float)engine->state.y) / engine->height, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		eglSwapBuffers(engine->display, engine->surface);
	}

	/**
	* 現在ディスプレイに関連付けられている EGL コンテキストを削除します。
	*/
	static void engine_term_display(struct ap::engine* engine) {
		if (engine->display != EGL_NO_DISPLAY) {
			eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (engine->context != EGL_NO_CONTEXT) {
				eglDestroyContext(engine->display, engine->context);
			}
			if (engine->surface != EGL_NO_SURFACE) {
				eglDestroySurface(engine->display, engine->surface);
			}
			eglTerminate(engine->display);
		}
		engine->animating = 0;
		engine->display = EGL_NO_DISPLAY;
		engine->context = EGL_NO_CONTEXT;
		engine->surface = EGL_NO_SURFACE;
	}

	/**
	* 次の入力イベントを処理します。
	*/
	static int32_t engine_handle_input(struct ap::android_app* app, AInputEvent* event) {
		struct ap::engine* engine = (struct ap::engine*)app->userData;
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
			engine->state.x = AMotionEvent_getX(event, 0);
			engine->state.y = AMotionEvent_getY(event, 0);
			return 1;
		}
		return 0;
	}

	/**
	* 次のメイン コマンドを処理します。
	*/
	static void engine_handle_cmd(struct ap::android_app* app, int32_t cmd) {
		struct ap::engine* engine = (struct ap::engine*)app->userData;
		switch (cmd) {
		case APP_CMD_SAVE_STATE:
			// 現在の状態を保存するようシステムによって要求されました。保存してください。
			engine->app->savedState = malloc(sizeof(struct ap::saved_state));
			*((struct ap::saved_state*)engine->app->savedState) = engine->state;
			engine->app->savedStateSize = sizeof(struct ap::saved_state);
			break;
		case APP_CMD_INIT_WINDOW:
			// ウィンドウが表示されています。準備してください。
			if (engine->app->window != NULL) {
				engine_init_display(engine);
				engine_draw_frame(engine);
			}
			break;
		case APP_CMD_TERM_WINDOW:
			// ウィンドウが非表示または閉じています。クリーン アップしてください。
			engine_term_display(engine);
			break;
		case APP_CMD_GAINED_FOCUS:
			// アプリがフォーカスを取得すると、加速度計の監視を開始します。
			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_enableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
				// 目標は 1 秒ごとに 60 のイベントを取得することです (米国)。
				ASensorEventQueue_setEventRate(engine->sensorEventQueue,
					engine->accelerometerSensor, (1000L / 60) * 1000);
			}
			break;
		case APP_CMD_LOST_FOCUS:
			// アプリがフォーカスを失うと、加速度計の監視を停止します。
			// これにより、使用していないときのバッテリーを節約できます。
			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_disableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
			}
			// また、アニメーションを停止します。
			engine->animating = 0;
			engine_draw_frame(engine);
			break;
		}
	}
}
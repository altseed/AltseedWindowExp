#include "from_sample.h"
#include "ap.Window_Impl_Android.h"


namespace ap
{
	int engine_init_display(struct ap::engine* engine) {
		// OpenGL ES �� EGL �̏�����

		/*
		* �ړI�̍\���̑����������Ŏw�肵�܂��B
		* �ȉ��ŁA�I���X�N���[�� �E�B���h�E��
		* �݊����̂���A�e�F�Œ� 8 �r�b�g�̃R���|�[�l���g�� EGLConfig ��I�����܂�
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

		/* �����ŁA�A�v���P�[�V�����͖ړI�̍\����I�����܂��B���̃T���v���ł́A
		* ���o�����ƈ�v����ŏ��� EGLConfig ��
		* �I������P���ȑI���v���Z�X������܂� */
		eglChooseConfig(display, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID �́AANativeWindow_setBuffersGeometry() ��
		* ����Ď󂯎���邱�Ƃ��ۏ؂���Ă��� EGLConfig �̑����ł��B
		* EGLConfig ��I�������炷���ɁAANativeWindow �o�b�t�@�[����v�����邽�߂�
		* EGL_NATIVE_VISUAL_ID ���g�p���Ĉ��S�ɍč\���ł��܂��B*/
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

		// GL �̏�Ԃ����������܂��B
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		glEnable(GL_CULL_FACE);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_DEPTH_TEST);

		return 0;
	}

	/**
	* ���݃f�B�X�v���C�Ɋ֘A�t�����Ă��� EGL �R���e�L�X�g���폜���܂��B
	*/
	void engine_term_display(struct ap::engine* engine) {
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
		engine->display = EGL_NO_DISPLAY;
		engine->context = EGL_NO_CONTEXT;
		engine->surface = EGL_NO_SURFACE;
	}

	/**
	* ���̓��̓C�x���g���������܂��B
	*/
	int32_t engine_handle_input(struct ap::android_app* app, AInputEvent* event) {
		struct ap::engine* engine = (struct ap::engine*)app->userData;
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
			engine->state.x = AMotionEvent_getX(event, 0);
			engine->state.y = AMotionEvent_getY(event, 0);
			return 1;
		}
		return 0;
	}

	/**
	* ���̃��C�� �R�}���h���������܂��B
	*/
	void engine_handle_cmd(struct ap::android_app* app, int32_t cmd) {
		struct ap::engine* engine = (struct ap::engine*)app->userData;
		switch (cmd) {
		case APP_CMD_SAVE_STATE:
			// ���݂̏�Ԃ�ۑ�����悤�V�X�e���ɂ���ėv������܂����B�ۑ����Ă��������B
			engine->app->savedState = malloc(sizeof(struct ap::saved_state));
			*((struct ap::saved_state*)engine->app->savedState) = engine->state;
			engine->app->savedStateSize = sizeof(struct ap::saved_state);
			break;
		case APP_CMD_INIT_WINDOW:
			// �E�B���h�E���\������Ă��܂��B�������Ă��������B
			if (engine->app->window != NULL) {
				engine_init_display(engine);
				//draw(engine);
			}
			break;
		case APP_CMD_TERM_WINDOW:
			// �E�B���h�E����\���܂��͕��Ă��܂��B�N���[�� �A�b�v���Ă��������B
			engine_term_display(engine);
			break;
		case APP_CMD_GAINED_FOCUS:
			// �A�v�����t�H�[�J�X���擾�����

			break;
		case APP_CMD_LOST_FOCUS:
			// �A�v�����t�H�[�J�X��������

			break;
		}
	}


	void draw(struct ap::engine* engine)
	{
		glClearColor(0.3f, 0.1f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(engine->display, engine->surface);
	}
}
#pragma once 
#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

namespace ap
{

	enum {
		/**
		* �A�v���̃��C�� �X���b�h����擾�����R�}���h�� Looper �f�[�^ ID �ł��B
		* ����� ALooper_pollOnce() ����̎��ʎq�Ƃ��ĕԂ���܂��B���̎��ʎq�̃f�[�^�́A
		* android_poll_source �\���̂ւ̃|�C���^�[�ł��B
		* ������ android_app_read_cmd()
		* ����� android_app_exec_cmd() �Ŏ擾���A�����ł��܂��B
		*/
		LOOPER_ID_MAIN = 1,

		/**
		* �A�v���P�[�V�����̃E�B���h�E�� AInputQueue ����擾�����C�x���g�� Looper �f�[�^ ID �ł��B
		* ����� ALooper_pollOnce() �����
		* ���ʎq�Ƃ��ĕԂ���܂��B���̎��ʎq�̃f�[�^��
		* android_poll_source �\���̂ւ̃|�C���^�[�ł��B������ android_app �� inputQueue �I�u�W�F�N�g
		* ����ǂݎ��܂��B
		*/
		LOOPER_ID_INPUT = 2,

		/**
		* ���[�U�[��`���ꂽ ALooper �̍ŏ��̎��ʎq�B
		*/
		LOOPER_ID_USER = 3,
	};

	enum {
		/**
		* ���C�� �X���b�h����̃R�}���h: AInputQueue ���ύX����܂����B���̃R�}���h�̏����̍ہA
		* android_app->inputQueue �͐V�����L���[ (�܂��� NULL)
		* �ɍX�V����܂��B
		*/
		APP_CMD_INPUT_CHANGED,

		/**
		* ���C�� �X���b�h����̃R�}���h: �V���� ANativeWindow ���g�p���鏀�����ł��܂����B���̃R�}���h����M����ہA
		* android_app->window �ɐV�����E�B���h�E��ʂ�
		* �܂܂�܂��B
		*/
		APP_CMD_INIT_WINDOW,

		/**
		* ���C�� �X���b�h����̃R�}���h: ������ ANativeWindow ���I������K�v��
		* ����܂��B���̃R�}���h����M����ۂɂ� android_app->window �ɂ͈�������
		* �����̃E�B���h�E���܂܂�Ă���Aandroid_app_exec_cmd
		���Ăяo�������    * NULL �ɐݒ肳��܂��B
		*/
		APP_CMD_TERM_WINDOW,

		/**
		* ���C�� �X���b�h����̃R�}���h: ���݂� ANativeWindow �̃T�C�Y���ύX����܂����B
		* �V�����T�C�Y���g�p���čĕ`�悵�Ă��������B
		*/
		APP_CMD_WINDOW_RESIZED,

		/**
		* ���C�� �X���b�h����̃R�}���h: ���̃V�X�e���ł́A���݂� ANativeWindow ���ĕ`��
		* �����K�v������܂��B�ꎞ�I�ȕ`��̕s���������邽�߂ɂ́Aandroid_app_exec_cmd()
		* �ɓn���O�ɃE�B���h�E���ĕ`�悷��K�v������܂��B
		*/
		APP_CMD_WINDOW_REDRAW_NEEDED,

		/**
		* ���C�� �X���b�h����̃R�}���h: �E�B���h�E�̃R���e���c�̈悪�ύX (�\���܂��͔�\��
		* �̃\�t�g���̓E�B���h�E�Ȃǂ���) ����܂����B�V�����R���e���c�̈�́A
		* android_app::contentRect �ɂ���܂��B
		*/
		APP_CMD_CONTENT_RECT_CHANGED,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B �E�B���h�E�����̓t�H�[�J�X
		* ���擾���܂����B
		*/
		APP_CMD_GAINED_FOCUS,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B �E�B���h�E�����̓t�H�[�J�X
		* �������܂����B
		*/
		APP_CMD_LOST_FOCUS,

		/**
		* ���C�� �X���b�h����̃R�}���h: ���݂̃f�o�C�X�\�����ύX����܂����B
		*/
		APP_CMD_CONFIG_CHANGED,

		/**
		* ���C�� �X���b�h����̃R�}���h: �V�X�e�� ���������s�����Ă��܂��B
		* �������̎g�p�ʂ����炵�Ă��������B
		*/
		APP_CMD_LOW_MEMORY,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B���J�n����܂����B
		*/
		APP_CMD_START,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B���ĊJ����܂����B
		*/
		APP_CMD_RESUME,

		/**
		* ���C�� �X���b�h����̃R�}���h: �K�v�ɉ����Č�ŕ����ł���悤�ɁA�A�v����
		* �V�����ۑ���Ԃ𐶐�����K�v������܂��B��Ԃ�ۑ������ꍇ��
		* malloc �����蓖�āAandroid_app.savedStateSize �̃T�C�Y��
		* android_app.savedState �ɔz�u���܂��B��ŉ��
		* ����܂��B
		*/
		APP_CMD_SAVE_STATE,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B���ꎞ��~���܂����B
		*/
		APP_CMD_PAUSE,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B����~���܂����B
		*/
		APP_CMD_STOP,

		/**
		* ���C�� �X���b�h����̃R�}���h: �A�v���̃A�N�e�B�r�e�B���j������Ă���A
		* ���s����O�ɁA�A�v�� �X���b�h���N���[�� �A�b�v���ďI������̂�ҋ@���Ă��܂��B
		*/
		APP_CMD_DESTROY,
	};

	struct android_poll_source {
		//���̃\�[�X�̎��ʎq�́ALOOPER_ID_MAIN �܂��� 
		//LOOPER_ID_INPUT �ł��B
		int32_t id;

		// ���� ident �Ɋ֘A�t�����Ă��� android_app�B
		struct android_app* app;

		//���̃\�[�X����̃f�[�^�̕W���v���Z�X�����s���邽�߂�
		//�Ăяo���֐��B
		void(*process)(struct android_app* app, struct android_poll_source* source);
	};



	struct android_app {
		//�K�؂ȏꍇ�ɂ́A�A�v���P�[�V�����͂��̏�ԃI�u�W�F�N�g�Ƀ|�C���^�[�����킹�邱�Ƃ�
		//�ł��܂��B
		void* userData;

		//���C�� �A�v�� �R�}���h (APP_CMD_*) ���������邽�߂̊֐�����͂��܂�
		void(*onAppCmd)(struct android_app* app, int32_t cmd);

		//���̓C�x���g���������邽�߂̊֐�����͂��܂��B�����_��
		// �C�x���g�͊��Ƀf�B�X�p�b�`����Ă���A�Ԃ����Ƃ��ɂ͊������Ă��܂��B
		// �C�x���g�������������Ƃ�����ꍇ�� 1 ��Ԃ��A���ׂĂ̊���̃f�B�X�p�b�`�ɂ��Ă�
		// 0 ��Ԃ��܂��B
		int32_t(*onInputEvent)(struct android_app* app, AInputEvent* event);

		// ���̃A�v�������s����Ă��� ANativeActivity �I�u�W�F�N�g �C���X�^���X�B
		ANativeActivity* activity;

		// ���̃A�v�������s����Ă��錻�݂̍\���B
		AConfiguration* config;

		// ���ꂪ�쐬���Ɏw�肳�ꂽ�Ō�̃C���X�^���X�̕ۑ��̏�Ԃł��B
		// ��Ԃ��Ȃ��ꍇ�� NULL �ł��B�K�v�ɉ����Ďg�p�ł��܂��B
		//�������[�́A��������ʒu�� APP_CMD_RESUME �ɑ΂��� android_app_exec_cmd()
		// ���Ăяo���܂ŕێ�����AsavedState �� NULL �ɐݒ肳��܂��B
		// �����̕ϐ��́ANULL �ɏ����������ʒu�� APP_CMD_SAVE_STATE ��
		//��������ꍇ�ɂ̂ݕύX�����K�v������A��Ԃ� malloc �ɂ���
		//���������ɔz�u���܂��B�������邱�ƂŁA��Ń������[��
		//�������܂��B
		void* savedState;
		size_t savedStateSize;

		// �A�v���̃X���b�h�Ɋ֘A�t�����Ă��� ALooper�B
		ALooper* looper;

		// ���ꂪ�ANULL �ȊO�̏ꍇ�ɃA�v�������[�U�[���̓C�x���g��
		// ��M������̓L���[�ł��B
		AInputQueue* inputQueue;

		// ���ꂪ�ANULL �ȊO�̏ꍇ�ɃA�v�����`��ł���E�B���h�E��ʂł��B
		ANativeWindow* window;

		// �E�B���h�E�̌��݂̃R���e���c�̈�́A���[�U�[�ɑ΂��ĕ\�������
		// �E�B���h�E�̃R���e���c���z�u�����ꏊ�ł��B
		ARect contentRect;

		// �A�v���̃A�N�e�B�r�e�B�̌��݂̏�Ԃ́AAPP_CMD_START�A
		// APP_CMD_RESUME�AAPP_CMD_PAUSE�A�܂��� APP_CMD_STOP �̂����ꂩ�ł��B�ȉ����Q�Ƃ��Ă��������B
		int activityState;

		// ����́A�A�v���P�[�V������ NativeActivity �̔j���������
		// �A�v�� �X���b�h�̊����̑ҋ@���̓[���ȊO�ɂȂ�܂��B
		int destroyRequested;

		// -------------------------------------------------
		// �ȉ��̓O���[ �R�[�h�� "�l" �̎����ł��B

		pthread_mutex_t mutex;
		pthread_cond_t cond;

		int msgread;
		int msgwrite;

		pthread_t thread;

		struct android_poll_source cmdPollSource;
		struct android_poll_source inputPollSource;

		int running;
		int stateSaved;
		int destroyed;
		int redrawNeeded;
		AInputQueue* pendingInputQueue;
		ANativeWindow* pendingWindow;
		ARect pendingContentRect;
	};




	struct saved_state {
		float angle;
		int32_t x;
		int32_t y;
	};

	/**
	* �A�v���̕ۑ���Ԃł��B
	*/
	struct engine {
		struct android_app* app;

		ASensorManager* sensorManager;
		const ASensor* accelerometerSensor;
		ASensorEventQueue* sensorEventQueue;

		int animating;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
		struct saved_state state;
	};
}
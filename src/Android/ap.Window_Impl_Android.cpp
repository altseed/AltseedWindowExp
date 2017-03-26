#include "ap.AndroidNativeActivityGlue.h"
#include "ap.Window_Impl_Android.h"

/*
* Copyright (C) 2010 The Android Open Source Project
*
* Apache License Version 2.0 (�u�{���C�Z���X�v) �Ɋ�Â��ă��C�Z���X����܂��B;
* �{���C�Z���X�ɏ������Ȃ��ꍇ�͂��̃t�@�C�����g�p�ł��܂���B
* �{���C�Z���X�̃R�s�[�́A�ȉ��̏ꏊ�������ł��܂��B
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* �K�p�����@�߂܂��͏��ʂł̓��ӂɂ���Ė������Ȃ�����A�{���C�Z���X�Ɋ�Â��ĔЕz�����\�t�g�E�F�A�́A
* �����َ����킸�A�����Ȃ�ۏ؂��������Ȃ��Ɍ���̂܂�
* �Еz����܂��B
* �{���C�Z���X�ł̌����Ɛ������K�肵���������ẮA
* �{���C�Z���X���Q�Ƃ��Ă��������B
*
*/
//�ꕔ����

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
		struct android_app* state =(struct android_app*) parameter.UserData[0];

		memset(&engine, 0, sizeof(engine));
		state->userData = &engine;
		state->onAppCmd = engine_handle_cmd;
		state->onInputEvent = engine_handle_input;
		engine.app = state;

		if (state->savedState != NULL) {
			// �ȑO�̕ۑ���Ԃ𕜌����Ă��������B
			engine.state = *(struct saved_state*)state->savedState;
		}

		return ErrorCode::OK;
	}

	bool Window_Impl_Android::DoEvent()
	{
		struct android_app* state = engine.app;
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident = ALooper_pollAll(0, NULL, &events,
			(void**)&source)) >= 0) {

			// ���̃C�x���g���������܂��B
			if (source != NULL) {
				source->process(state, source);
			}


			// �I�����邩�ǂ����m�F���܂��B
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
			// �f�B�X�v���C������܂���B
			// �G���[����������H
			return;
		}
	}
}
/*
* Copyright (C) 2010 The Android Open Source Project
*
* Apache License Version 2.0 (「本ライセンス」) に基づいてライセンスされます。;
* 本ライセンスに準拠しない場合はこのファイルを使用できません。
* 本ライセンスのコピーは、以下の場所から入手できます。
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* 適用される法令または書面での同意によって命じられない限り、本ライセンスに基づいて頒布されるソフトウェアは、
* 明示黙示を問わず、いかなる保証も条件もなしに現状のまま
* 頒布されます。
* 本ライセンスでの権利と制限を規定した文言ついては、
* 本ライセンスを参照してください。
*
*/


#include "ap.Window_Impl_Android.h"
#include "ap.AndroidNativeActivityGlue.h"

/**
* これは、android_native_app_glue を使用しているネイティブ アプリケーション
* のメイン エントリ ポイントです。それ自体のスレッドでイベント ループによって実行され、
* 入力イベントを受け取ったり他の操作を実行したりします。
*/
void android_main(struct android_app* state) {
	ap::Window_Impl_Android* imp = new ap::Window_Impl_Android();
	ap::WindowInitializationParameter* param = new ap::WindowInitializationParameter();
	param->UserData[0] = state;
	imp->Initialize(*param);

	while (imp->DoEvent()) {
		
		imp->Update();

		struct engine* engine = &(imp->engine);
		glClearColor(0.3f, 0.8f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(engine->display, engine->surface);
	}



}

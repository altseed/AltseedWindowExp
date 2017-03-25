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

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

#include "ap.Window_Impl_Android.h"

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

	// ループはスタッフによる開始を待っています。

	while (1) {
		// 保留中のすべてのイベントを読み取ります。

		imp->DoEvent();

		// アニメーションしない場合、無期限にブロックしてイベントが発生するのを待ちます。
		// アニメーションする場合、すべてのイベントが読み取られるまでループしてから続行します
		// アニメーションの次のフレームを描画します。
		imp->Update();

	}
}

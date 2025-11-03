#include "pch.h"
#include "resource.h"
#include "version.hpp"
#include "app_interface.hpp"
#include "hive.hpp"
#include "utils.hpp"
#include "window/richedit.hpp"
#include "window/richedit_window.hpp"
#include "window/project_note.hpp"
#include "window/sharing_note.hpp"
#include "io_base.hpp"
#include "config_io.hpp"
#include "app.hpp"

namespace apn::memo
{
	//
	// エントリポイントです。
	//
	EXTERN_C BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved)
	{
		switch (reason)
		{
		case DLL_PROCESS_ATTACH:
			{
				::DisableThreadLibraryCalls(hive.instance = instance);
#ifdef _DEBUG
		// デバッグ用のコードです。
		{
//			if (0)
			{
				// カスタムロガーを設定します。
				static struct Logger : my::Tracer::Logger {
					virtual void output(LPCTSTR raw, LPCTSTR text) override {
						// SHIFTキーが押されているときだけログを出力します。
						if (::GetKeyState(VK_SHIFT) < 0) ::OutputDebugString(text);
					}
				} logger;
				my::Tracer::logger = &logger;
			}
		}
#endif
				break;
			}
		case DLL_PROCESS_DETACH:
			{
				break;
			}
		}

		return TRUE;
	}

	//
	// プラグインDLL初期化関数です。
	//
	EXTERN_C bool InitializePlugin(DWORD version)
	{
		return !!app->on_init();
	}

	//
	// プラグインDLL終了関数です。
	//
	EXTERN_C void UninitializePlugin()
	{
		return (void)app->on_exit();
	}

	//
	// プラグイン情報を登録します。
	//
	EXTERN_C __declspec(dllexport) void RegisterPlugin(HOST_APP_TABLE* host)
	{
		return (void)app->on_register_plugin(host);
	}
}

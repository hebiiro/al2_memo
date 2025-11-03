#pragma once

namespace apn::memo
{
	//
	// このクラスはアプリケーションのインターフェイスです。
	//
	struct app_interface_t {
		virtual BOOL on_init() = 0;
		virtual BOOL on_exit() = 0;
		virtual BOOL on_register_plugin(HOST_APP_TABLE* host) = 0;
		virtual BOOL on_project_load(PROJECT_FILE* project) = 0;
		virtual BOOL on_project_save(PROJECT_FILE* project) = 0;
	} *app = nullptr;
}

#pragma once

namespace apn::memo
{
	//
	// このクラスはアプリケーションです。
	//
	inline struct app_t : app_interface_t
	{
		//
		// コンストラクタです。
		//
		app_t() { app = this; }

		//
		// 初期化処理を実行します。
		//
		virtual BOOL on_init() override
		{
			MY_TRACE_FUNC("");

			// リッチエディットコントロールを使用できるようにします。
			::LoadLibraryW(L"riched20.dll");

			// 各種初期化処理を実行します。
			if (!config_io.init()) return FALSE;
			if (!sharing_note.init()) return FALSE;
			if (!project_note.init()) return FALSE;

			// コンフィグをファイルから読み込みます。
			if (!config_io.read()) MY_TRACE("コンフィグの読み込みに失敗しました\n");

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		virtual BOOL on_exit() override
		{
			MY_TRACE_FUNC("");

			// コンフィグをファイルに書き込みます。
			config_io.write();

			// 各種終了処理を実行します。
			project_note.exit();
			sharing_note.exit();
			config_io.exit();

			return TRUE;
		}

		//
		// プラグインを登録します。
		//
		virtual BOOL on_register_plugin(HOST_APP_TABLE* host) override
		{
			MY_TRACE_FUNC("");

			// プラグインの情報を設定します。
			host->set_plugin_information(version.information.c_str());

			// ウィンドウを登録します。
			host->register_window_client(L"共通ノート", sharing_note);
			host->register_window_client(L"プロジェクトノート", project_note);

			// 編集ハンドルを作成します。
			hive.edit_handle = host->create_edit_handle();

			// プロジェクト読み込み関数を登録します。
			host->register_project_load_handler(
				[](PROJECT_FILE* project){ return (void)app->on_project_load(project); });

			// プロジェクト書き込み関数を登録します。
			host->register_project_save_handler(
				[](PROJECT_FILE* project){ return (void)app->on_project_save(project); });

			return TRUE;
		}

		//
		// プロジェクトノートのキー名(UTF-8)です。
		//
		inline static constexpr auto c_project_note = u8"project_note";

		//
		// プロジェクトファイルから読み込みます。
		//
		virtual BOOL on_project_load(PROJECT_FILE* project) override
		{
			try
			{
				// プロジェクトノートを読み込めた場合は
				if (auto json_string = project->get_param_string((LPCSTR)c_project_note))
				{
					// json形式からプレーン文字列形式に変換します。
					auto wcs = my::cp_to_wide(my::json::n_json::parse(json_string).get<std::string>(), CP_UTF8);

					// プロジェクトノートをセットします。
					::SetWindowTextW(project_note.richedit, wcs.c_str());

					return TRUE;
				}
			}
			catch (...)
			{
			}

			// プロジェクトノートをリセットします。
			::SetWindowTextW(project_note.richedit, L"");

			return FALSE;
		}

		//
		// プロジェクトファイルに書き込みます。
		//
		virtual BOOL on_project_save(PROJECT_FILE* project) override
		{
			try
			{
				// プロジェクトノートが有効の場合は
				if (auto wcs = my::get_window_text(project_note.richedit); wcs.length())
				{
					// プレーン文字列形式からjson形式に変換します。
					auto json_string = my::json::n_json(my::wide_to_cp(wcs, CP_UTF8)).dump();

					// プロジェクトノートを書き込みます。
					project->set_param_string((LPCSTR)c_project_note, json_string.c_str());

					return TRUE;
				}
			}
			catch (...)
			{
			}

			return FALSE;
		}
	} app_impl;
}

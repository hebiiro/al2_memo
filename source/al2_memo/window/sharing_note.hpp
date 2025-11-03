#pragma once

namespace apn::memo
{
	//
	// このクラスは共通ノートウィンドウです。
	// このウィンドウはプラグインウィンドウのように振る舞います。
	//
	inline struct sharing_note_t : richedit_window_t
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			return __super::init();
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return __super::exit();
		}

		//
		// ウィンドウプロシージャです。
		//
		virtual LRESULT on_wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_CREATE:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					// 先に基本クラスの処理を実行します。
					auto lr = __super::on_wnd_proc(hwnd, message, wParam, lParam);
					if (lr == -1) return lr;

					try
					{
						// 共通ノートファイルが存在する場合は
						if (std::filesystem::exists(hive.sharing_note_file_name))
						{
							// txtファイルから文字列を読み込みます。
							auto sharing_text = utils::read_txt(hive.sharing_note_file_name);

							// リッチエディットコントロールにセットします。
							::SetWindowTextW(richedit, my::cp_to_wide(sharing_text, CP_UTF8).c_str());
						}
					}
					catch (...)
					{
						hive.message_box(L"共通ノートの読み込みに失敗しました");
					}

					return lr;
				}
			case WM_DESTROY:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					try
					{
						// リッチエディットコントロールから文字列を取得します。
						auto sharing_text = my::wide_to_cp(my::get_window_text(richedit), CP_UTF8);

						// txtファイルに文字列を書き込みます。
						utils::write_txt(hive.sharing_note_file_name, sharing_text);
					}
					catch (...)
					{
						hive.message_box(L"共通ノートの書き込みに失敗しました");
					}

					break;
				}
			}

			return __super::on_wnd_proc(hwnd, message, wParam, lParam);
		}
	} sharing_note;
}

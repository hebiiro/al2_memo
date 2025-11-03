#pragma once

namespace apn::memo
{
	//
	// このクラスはリッチエディットコントロールです。
	//
	struct richedit_t : my::Window
	{
		inline static constexpr struct command_id_t {
			inline static constexpr auto c_cut = 1;
			inline static constexpr auto c_copy = 2;
			inline static constexpr auto c_paste = 3;
			inline static constexpr auto c_erase = 4;
			inline static constexpr auto c_select_all = 5;
			inline static constexpr auto c_undo = 6;
			inline static constexpr auto c_redo = 7;
			inline static constexpr auto c_choose_font = 8;
		} c_command_id;

		//
		// このクラスはリッチエディットコントロールで使用するフォントです。
		//
		struct font_t {
			//
			// フォント名です。
			//
			std::wstring name = L"Meiryo";

			//
			// フォントの高さです。
			//
			int32_t height = -32;

			//
			// フォントの太さです。
			//
			int32_t weight = 0;

			//
			// TRUEの場合は斜体になります。
			//
			BOOL flag_italic = FALSE;

			//
			// TRUEの場合は下線が付きます。
			//
			BOOL flag_underline = FALSE;

			//
			// フォントのピッチ(可変か固定か)です。
			//
			int32_t pitch = 0;

			//
			// フォントのハンドルです。
			//
			my::gdi::unique_ptr<HFONT> handle;
		} font;

		//
		// 初期化処理を実行します。
		//
		BOOL init(HWND parent)
		{
			MY_TRACE_FUNC("{/hex}", parent);

			// ウィンドウを作成します。
			if (!__super::create(
				WS_EX_CLIENTEDGE,
				RICHEDIT_CLASSW,
				nullptr,
				WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL |
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
				ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL |
				ES_AUTOVSCROLL | ES_AUTOHSCROLL |
				ES_DISABLENOSCROLL | ES_SELECTIONBAR | ES_SAVESEL,
				0, 0, 0, 0,
				parent, nullptr, hive.instance, nullptr))
			{
				hive.message_box(L"リッチエディットコントロールの作成に失敗しました");

				return FALSE;
			}

			// イベントマスクを設定します。
			auto event_mask = ::SendMessage(*this, EM_GETEVENTMASK, 0, 0);
			event_mask |= ENM_MOUSEEVENTS;
			::SendMessage(*this, EM_SETEVENTMASK, 0, event_mask);

			// 言語オプションを設定します。
			auto lang_options = ::SendMessage(*this, EM_GETLANGOPTIONS, 0, 0); 
//			lang_options &= ~IMF_DUALFONT;
//			lang_options &= ~IMF_AUTOFONT;
//			lang_options &= ~IMF_AUTOFONTSIZEADJUST;
			lang_options |= IMF_UIFONTS;
			::SendMessage(*this, EM_SETLANGOPTIONS, 0, lang_options);

			// 余白を設定します。
			auto margin = RECT { 18, 6, 6, 6 };
			::SendMessage(*this, EM_SETRECT, TRUE, (LPARAM)&margin);
//			::SendMessage(*this, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(6, 6));

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			// ウィンドウを破壊します。
			return destroy();
		}

		//
		// ノードからコンフィグを読み込みます。
		//
		BOOL read_config(n_json& root)
		{
			MY_TRACE_FUNC("");

			n_json font_node;
			read_child_node(root, "font", font_node);
			read_string(font_node, "name", font.name);
			read_int(font_node, "height", font.height);
			read_int(font_node, "weight", font.weight);
			read_bool(font_node, "flag_italic", font.flag_italic);
			read_bool(font_node, "flag_underline", font.flag_underline);
			read_int(font_node, "pitch", font.pitch);

			update_font(TRUE);

			return TRUE;
		}

		//
		// ノードにコンフィグを書き込みます。
		//
		BOOL write_config(n_json& root)
		{
			MY_TRACE_FUNC("");

			n_json font_node;
			write_string(font_node, "name", font.name);
			write_int(font_node, "height", font.height);
			write_int(font_node, "weight", font.weight);
			write_bool(font_node, "flag_italic", font.flag_italic);
			write_bool(font_node, "flag_underline", font.flag_underline);
			write_int(font_node, "pitch", font.pitch);
			write_child_node(root, "font", font_node);

			return TRUE;
		}

		//
		// 論理フォントを返します。
		//
		LOGFONTW get_log_font()
		{
			auto lf = LOGFONTW {};
			lf.lfHeight = font.height;
			lf.lfWeight = font.weight;
			lf.lfItalic = font.flag_italic;
			lf.lfUnderline = font.flag_underline;
			lf.lfCharSet = DEFAULT_CHARSET;
			lf.lfPitchAndFamily = font.pitch;
			wcscpy_s(lf.lfFaceName, font.name.c_str());
			return lf;
		}

		//
		// 論理フォントを設定します。
		//
		void set_log_font(const LOGFONTW& lf)
		{
			font.height = lf.lfHeight;
			font.weight = lf.lfWeight;
			font.flag_italic = !!lf.lfItalic;
			font.flag_underline = !!lf.lfUnderline;
			font.pitch = lf.lfPitchAndFamily;
			font.name = lf.lfFaceName;
		}

		//
		// リッチエディットコントロールのフォントを更新します。
		//
		void update_font(BOOL redraw)
		{
			// 論理フォントを取得します。
			auto lf = get_log_font();

			// フォントを再作成します。
			font.handle.reset(::CreateFontIndirectW(&lf));

			// フォントをセットします。
			::SendMessage(*this, WM_SETFONT, (WPARAM)font.handle.get(), redraw);
		}

		//
		// フォント選択ダイアログを表示します。
		//
		BOOL choose_font()
		{
			// 論理フォントを取得します。
			auto lf = get_log_font();

			// フォント選択ダイアログを表示します。
			CHOOSEFONTW cf = { sizeof(cf) };
			cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_NOVERTFONTS;
			cf.hwndOwner = *this;
			cf.lpLogFont = &lf;
			if (!::ChooseFont(&cf)) return FALSE;

			// 論理フォントをセットします。
			set_log_font(lf);

			// リッチエディットコントロールのフォントを更新します。
			update_font(TRUE);

			return TRUE;
		}

		//
		// 親ウィンドウに送られたリッチエディットコントロールのコマンドを処理します。
		//
		LRESULT on_command_reflection(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			auto code = HIWORD(wParam);
			auto id = LOWORD(wParam);
			auto control = (HWND)lParam;

			switch (id)
			{
			case c_command_id.c_cut: return ::SendMessage(*this, WM_CUT, 0, 0);
			case c_command_id.c_copy: return ::SendMessage(*this, WM_COPY, 0, 0);
			case c_command_id.c_paste: return ::SendMessage(*this, WM_PASTE, 0, 0);
			case c_command_id.c_erase: return ::SendMessage(*this, WM_KEYDOWN, VK_DELETE, 0);
			case c_command_id.c_select_all: return ::SendMessage(*this, EM_SETSEL, 0, -1);
			case c_command_id.c_undo: return ::SendMessage(*this, EM_UNDO, 0, 0);
			case c_command_id.c_redo: return ::SendMessage(*this, EM_REDO, 0, 0);
			case c_command_id.c_choose_font: return choose_font();
			}

			return 0;
		}

		//
		// 親ウィンドウに送られたリッチエディットコントロールの通知を処理します。
		//
		LRESULT on_notify_reflection(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			auto header = (NMHDR*)lParam;

			switch (header->code)
			{
			case EN_MSGFILTER:
				{
					auto msg_filter = (MSGFILTER*)lParam;

					switch (msg_filter->msg)
					{
					case WM_RBUTTONDOWN:
						{
							// リッチエディットコントロール用のコンテキストメニューを表示します。

							my::menu::unique_ptr<> menu(::CreatePopupMenu());

							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_undo, L"元に戻す(&U)");
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_redo, L"やり直し(&R)");
							::AppendMenuW(menu.get(), MF_SEPARATOR, 0, nullptr);
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_cut, L"切り取り(&T)");
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_copy, L"コピー(&C)");
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_paste, L"貼り付け(&P)");
							::AppendMenuW(menu.get(), MF_SEPARATOR, 0, nullptr);
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_erase, L"削除(&D)");
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_select_all, L"すべて選択(&A)");
							::AppendMenuW(menu.get(), MF_SEPARATOR, 0, nullptr);
							::AppendMenuW(menu.get(), MF_STRING, c_command_id.c_choose_font, L"フォントを選択(&F)");

							if (!::SendMessage(*this, EM_CANUNDO, 0, 0)) ::EnableMenuItem(menu.get(), c_command_id.c_undo, MF_DISABLED | MF_GRAYED);
							if (!::SendMessage(*this, EM_CANREDO, 0, 0)) ::EnableMenuItem(menu.get(), c_command_id.c_redo, MF_DISABLED | MF_GRAYED);

							auto pos = my::lp_to_pt(msg_filter->lParam);
							my::map_window_points(msg_filter->nmhdr.hwndFrom, nullptr, &pos);

							::TrackPopupMenuEx(menu.get(), 0, pos.x, pos.y, hwnd, nullptr);

							break;
						}
					}

					break;
				}
			}

			return 0;
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

					break;
				}
			case WM_DESTROY:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					break;
				}
			}

			return __super::on_wnd_proc(hwnd, message, wParam, lParam);
		}
	};
}

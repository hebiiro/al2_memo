#pragma once

namespace apn::memo
{
	//
	// このクラスはリッチエディットウィンドウです。
	// このウィンドウはリッチエディットコントロールを保持します。
	//
	struct richedit_window_t : my::Window
	{
		//
		// ウィンドウのクラス名です。
		//
		inline static constexpr auto c_class_name = L"apn::memo::richedit_window_t";

		//
		// リッチエディットコントロールです。
		//
		richedit_t richedit;

		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			// ウィンドウクラスを登録します。
			WNDCLASSEXW wc = { sizeof(wc) };
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = ::DefWindowProc;
			wc.hInstance = hive.instance;
			wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
			wc.lpszClassName = c_class_name;
			::RegisterClassExW(&wc);

			// ウィンドウを作成します。
			return create(
				WS_EX_NOPARENTNOTIFY,
				c_class_name,
				c_class_name,
				WS_POPUP,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				nullptr,
				nullptr,
				hive.instance,
				nullptr);
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
		// ウィンドウプロシージャです。
		//
		virtual LRESULT on_wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_CREATE:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					if (!richedit.init(hwnd))
					{
						MY_TRACE("リッチエディットコントロールの作成に失敗しました\n");

						return -1;
					}

					break;
				}
			case WM_DESTROY:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					richedit.exit();

					break;
				}
			case WM_SIZE:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					auto rc = my::get_client_rect(hwnd);
					auto x = rc.left;
					auto y = rc.top;
					auto w = my::get_width(rc);
					auto h = my::get_height(rc);
					auto flags = SWP_NOZORDER | SWP_ASYNCWINDOWPOS;
					if (::IsWindowVisible(hwnd)) flags |= SWP_SHOWWINDOW;

					::SetWindowPos(richedit, nullptr, x, y, w, h, flags);

					break;
				}
			case WM_COMMAND:
				{
					MY_TRACE_FUNC("WM_COMMAND, {/hex}, {/hex}", wParam, lParam);

					richedit.on_command_reflection(hwnd, message, wParam, lParam);

					break;
				}
			case WM_NOTIFY:
				{
					MY_TRACE_FUNC("WM_NOTIFY, {/hex}, {/hex}", wParam, lParam);

					richedit.on_notify_reflection(hwnd, message, wParam, lParam);

					break;
				}
			}

			return __super::on_wnd_proc(hwnd, message, wParam, lParam);
		}
	};
}

#pragma once

namespace apn::memo
{
	//
	// このクラスはコンフィグの入出力を担当します。
	//
	inline struct config_io_t : io_base_t
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			auto module_file_path = my::get_module_file_name(hive.instance);
			auto config_folder_path = module_file_path.parent_path() / L"al2" / L"config";

			path = hive.config_file_name = config_folder_path /
				module_file_path.filename().replace_extension(L".json");
			MY_TRACE_STR(hive.config_file_name);

			hive.sharing_note_file_name = config_folder_path / L"sharing_note.txt";

			// コンフィグフォルダを作成しておきます。
			try { std::filesystem::create_directories(config_folder_path); } catch (...) {}

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// 更新処理を実行します。
		//
		BOOL update()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// ノードからコンフィグを読み込みます。
		//
		virtual BOOL read_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			{
				n_json node;
				my::json::read_child_node(root, "sharing_note", node);
				sharing_note.richedit.read_config(node);
			}

			{
				n_json node;
				my::json::read_child_node(root, "project_note", node);
				project_note.richedit.read_config(node);
			}

			return TRUE;
		}

		//
		// ノードにコンフィグを書き込みます。
		//
		virtual BOOL write_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			{
				n_json node;
				sharing_note.richedit.write_config(node);
				write_child_node(root, "sharing_note", node);
			}

			{
				n_json node;
				project_note.richedit.write_config(node);
				write_child_node(root, "project_note", node);
			}

			return TRUE;
		}
	} config_io;
}

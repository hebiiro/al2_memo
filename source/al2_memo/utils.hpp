#pragma once

namespace apn::memo::utils
{
	//
	// txtファイルの内容(文字列)を返します。
	//
	std::string read_txt(const std::filesystem::path& path)
	{
		// バイナリ入力ストリームを開きます。
		std::ifstream ifs(path, std::ios_base::binary);
		if (!ifs) return {};

		// ファイルサイズを取得します。
		auto file_size = (size_t)std::filesystem::file_size(path);

		// バッファを確保します。
		std::string buffer(file_size, '\0');

		// 文字列をストリームから読み込みます。
		ifs.read(buffer.data(), file_size);

		// バッファを返します。
		return buffer;
	}

	//
	// txtファイルに文字列を書き込みます。
	//
	void write_txt(const std::filesystem::path& path, const std::string& s)
	{
		// バイナリ出力ストリームを開きます。
		std::ofstream ofs(hive.sharing_note_file_name, std::ios_base::binary);

		// 文字列をストリームに書き込みます。
		ofs.write(s.c_str(), s.length());
	}
}

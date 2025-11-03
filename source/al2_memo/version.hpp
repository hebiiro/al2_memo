#pragma once

namespace apn::memo
{
	//
	// このクラスはこのプログラムのバージョンを管理します。
	//
	inline struct Version {
		//
		// プラグイン名とプラグイン情報です。
		//
		const std::wstring name = L"🐍メモ帳";
		const std::wstring information = name + L"🔖r1"; // リリース番号を付与します。
	} version;
}

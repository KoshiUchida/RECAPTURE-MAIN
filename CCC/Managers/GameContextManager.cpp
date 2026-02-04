/**
 * @file   GameContextManager.cpp
 *
 * @brief  ゲームデータの管理クラスのファイル
 *
 * @author CatCode
 *
 * @date   2026/01/19
 * シーンを超えてデータを共有するための管理クラス
 * 
 * 取り扱うデータはこのソースファイルで予め登録しなければならない。
 * 
 * 2026/01/19
 * 作成
 */

#include "pch.h"
#include "GameContextManager.h"

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<GameContextManager> GameContextManager::s_Instance = nullptr;

	GameContextManager::GameContextManager()
	{
		this->Registration();
	}

	GameContextManager* GameContextManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new GameContextManager());

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	GameContextManager::~GameContextManager() = default;




	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //

	void GameContextManager::Registration()
	{
		// ---------------------------------------------------------------------- //
		// 使用データの定義はここで行う
		// ---------------------------------------------------------------------- //

		// 勝敗
		AddGameData("PlayerIsWin", false);

		// デバッグデータ
		AddGameData("BlockCount", 0);
	}
	


	// ---------------------------------------------------------------------- //
	// ゲームデータ
	// ---------------------------------------------------------------------- //

	void GameContextManager::AddGameData(const std::string& name, const std::any& data)
	{
		// データを登録
		m_Dates.emplace(name, data);
	}

	std::any GameContextManager::GetData(const std::string& name)
	{
		return m_Dates[name];
	}

	void GameContextManager::SetGameData(const std::string& name, const std::any& data)
	{
		// そのデータは登録されているか
		auto it = m_Dates.find(name);
		if (it == m_Dates.end()) return;

		// 設定
		it->second = data;
	}
}

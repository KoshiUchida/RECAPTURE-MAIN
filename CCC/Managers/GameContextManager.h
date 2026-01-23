/**
 * @file   GameContextManager.h
 *
 * @brief  ゲームデータの管理クラスのヘッダファイル
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

// 多重インクルードガード
#pragma once

#include <any>
#include <unordered_map>
#include <memory>

namespace CCC::Managers
{
	/// <summary>
	/// ゲームデータ管理クラス
	/// </summary>
	class GameContextManager
	{
		// ---------------------------------------------------------------------- //
		// シングルトンパターン関連
		// ---------------------------------------------------------------------- //
	private:
		// 実体を他のクラスが作れないように

		/// <summary>
		/// コンストラクタ
		/// </summary>
		GameContextManager();

		// このクラスの実体
		static std::unique_ptr<GameContextManager> s_Instance;

	public:
		/// <summary>
		/// 実体の取得
		/// </summary>
		/// <returns>リソース管理クラスへのポインタ</returns>
		static GameContextManager* GetInstance();



		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		~GameContextManager();
		
		
		
		// ---------------------------------------------------------------------- //
		// メンバ関数
		// ---------------------------------------------------------------------- //
	private:
		/// <summary>
		/// データの種別を登録する処理
		/// </summary>
		void Registration();


		// ---------------------------------------------------------------------- //
		// ゲームデータ
		// ---------------------------------------------------------------------- //
	private:
		using GameDatesCollection = std::unordered_map < std::string, std::any>;
		GameDatesCollection m_Dates;

		/// <summary>
		/// ゲームデータの登録
		/// </summary>
		/// <param name="name">登録名</param>
		/// <param name="data">登録するデータ</param>
		void AddGameData(const std::string& name, const std::any& data);

		/// <summary>
		/// ゲームデータの取得
		/// </summary>
		/// <param name="name">登録名</param>
		std::any GetData(const std::string& name);

	public:
		/// <summary>
		/// ゲームデータの取得
		/// </summary>
		/// <typeparam name="T">登録したデータの返し値の型/typeparam>
		/// <param name="name">登録名</param>
		template<class T>
		T GetDataAs(const std::string& name)
		{
			return std::any_cast<T>(GetData(name));
		}

		/// <summary>
		/// ゲームデータの設定
		/// </summary>
		/// <param name="name">登録名</param>
		/// <param name="data">設定するデータ</param>
		void SetGameData(const std::string& name, const std::any& data);
	};
}

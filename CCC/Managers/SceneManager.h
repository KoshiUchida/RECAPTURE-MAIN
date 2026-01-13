/**
 * @file   SceneManager.h
 *
 * @brief  シーンの管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/11/14
 * Gameクラスで必ず、更新処理と描画処理を行うこと
 * シーンの一元管理
 * m_IsEnd（ゲームの終了のフラグ）を
 * TRUEにするには、End関数を実行
 */

#pragma once
#ifndef SCENE_MANAGER_DEFINED
#define SCENE_MANAGER_DEFINED

#include <string>
#include <memory>

// TODO:後にObjectManagerを実装したら修正
//#include "ObjectManager.h"

namespace CCC
{
	namespace Managers
	{
		/// <summary>
		/// シーン管理クラス
		/// </summary>
		class SceneManager final
		{
			// ---------------------------------------------------------------------- //
			// シングルトンパターン関連
			// ---------------------------------------------------------------------- //
		private:
			// 他のクラスでこのクラスの実体を作成できないようにする

			/// <summary>
			/// コンストラクタ
			/// </summary>
			SceneManager();

			// このクラスの実体
			static std::unique_ptr<SceneManager> s_Instance;

		public:
			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns>シーン管理クラスへのポインタ</returns>
			static SceneManager* GetInstance();



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// 前方宣言
			struct Impl;

			// 定義をCPPにて行う
			std::unique_ptr<Impl> m_Impl;



			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			~SceneManager();

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime);

			/// <summary>
			/// 描画処理
			/// </summary>
			void Render();

			/// <summary>
			/// 現在のシーンの名前の取得
			/// </summary>
			/// <returns>現在のシーン名</returns>
			const std::string& GetCurrentSceneName() const;

			/// <summary>
			/// 現在のシーンの名前の取得
			/// </summary>
			/// <returns>現在のシーン名（wstring型）</returns>
			std::wstring GetCurrentSceneNameUTF16() const;


			/// <summary>
			/// シーン遷移の要求
			/// </summary>
			/// <param name="changeSceneName">遷移先のシーン名</param>
			void RequestSceneChange(const std::string& changeSceneName);

			/// <summary>
			/// 終了命令
			/// </summary>
			void End();
		};
	}
}

#endif // !SCENE_MANAGER_DEFINED

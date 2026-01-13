/**
 * @file   SceneBase.h
 *
 * @brief  シーンの基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/11/16
 * シーンを一元管理するための基底クラス
 * シーンの作成する場合はこのクラスを継承しなければならない
 * Initialize、Update、Redner、Finalize
 * 以上の関数は必ず実装（override）しなければならない
 * 
 * 作成したらSceneManager.cppに登録することを忘れずに
 * 
 * ChangeScene関数で、指定したシーンに遷移可能 
 */

#pragma once
#ifndef SCENE_BASE_DEFINED
#define SCENE_BASE_DEFINED

 // 親クラス
#include <CCC/Interfaces/IScene.h>

namespace CCC
{
	namespace Managers
	{
		// 前方宣言
		class SceneManager;
	}

	namespace Bases
	{
		/// <summary>
		/// シーンの基底クラス
		/// </summary>
		class SceneBase :
			public CCC::Interfaces::IScene
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			SceneBase();

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~SceneBase() noexcept;

			/// <summary>
			/// 初期化処理
			/// </summary>
			virtual void Initialize() override = 0;

			/// <summary>
			/// 更新処理
			/// </summary>
			virtual void Update(float elapsedTime) override = 0;

			/// <summary>
			/// 描画処理
			/// </summary>
			virtual void Render() override = 0;

			/// <summary>
			/// 終了処理
			/// </summary>
			virtual void Finalize() override = 0;

			/// <summary>
			/// シーンの変更
			/// </summary>
			/// <param name="nextSceneName">変更先のシーン名</param>
			void ChangeScene(const std::string& nextSceneName);



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// シーンマネージャへのポインタ
			CCC::Managers::SceneManager* mp_SceneManager;
		};
	}
}

#endif // !SCENE_BASE_DEFINED

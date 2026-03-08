/**
 * @file   ManagersHub.h
 *
 * @brief  管理中継クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/02/08
 *
 * 管理クラスを中継するためのクラス
 * シングルトン構造が中心で作られたシステムを変えるために実装
 *
 * 2026/02/08
 * 作成
 */

// 管理クラスを束ね、配布する管理クラス

// 多重インクルードガード
#pragma once

// C++標準ライブラリ
#include <memory>

namespace CCC
{
	// 前方宣言
	namespace Interfaces
	{
		class IColliderManager;
	}

	namespace Managers
	{
		class ManagersHub
		{
			// ---------------------------------------------------------------------- //
			// シングルトン構造
			// ---------------------------------------------------------------------- //
		private:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="p_ColliderManager">コライダー管理クラスへのポインタ</param>
			ManagersHub(CCC::Interfaces::IColliderManager* p_ColliderManager);

		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			~ManagersHub();

			/// <summary>
			/// 生成
			/// </summary>
			/// <param name="p_ColliderManager">コライダー管理クラスへのポインタ</param>
			static void Generate(CCC::Interfaces::IColliderManager* p_ColliderManager);

			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns></returns>
			static ManagersHub* Get();

			/// <summary>
			/// コライダー管理クラスの取得
			/// </summary>
			CCC::Interfaces::IColliderManager* GetColliderManager() const;



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			static std::unique_ptr<ManagersHub> s_Instance;	// 実体

			CCC::Interfaces::IColliderManager* mp_ColliderManager;	// コライダー管理クラスへのポインタ
		};
	}
}

/**
 * @file   ColliderManager.h
 *
 * @brief  コライダー管理クラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2026/02/05
 *
 * コライダーの衝突処理を管理するクラス
 *
 * 2026/02/05
 * 作成
 */

 // 多重インクルードガード
#pragma once

// 実装元クラス
#include <CCC/Interfaces/IColliderManager.h>

// C++標準ライブラリ
#include <vector>

namespace CCC
{
	// 前方宣言
	namespace Bases
	{
		class ColliderComponentBase;
	}

	namespace Managers
	{
		class ColliderManager :
			public CCC::Interfaces::IColliderManager
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			ColliderManager() noexcept;
			virtual ~ColliderManager() noexcept;

			/// <summary>
			/// 更新処理
			/// </summary>
			void Update() override final;

			/// <summary>
			/// コライダーコンポーネントの追加
			/// </summary>
			void AddCollider(CCC::Interfaces::IComponent* p_ColliderComponent) override final;

			/// <summary>
			/// 追加されていたコライダーコンポーネントを削除
			/// </summary>
			void Clear();



			// ---------------------------------------------------------------------- //
			// メンバ関数
			// ---------------------------------------------------------------------- //
		private:
			/// <summary>
			/// 衝突処理
			/// </summary>
			void Collision(CCC::Bases::ColliderComponentBase* p_ColliderA, CCC::Bases::ColliderComponentBase* p_ColliderB);

			/// <summary>
			/// 四角形同士の衝突処理
			/// </summary>
			void BoxToBox(CCC::Bases::ColliderComponentBase* p_ColliderA, CCC::Bases::ColliderComponentBase* p_ColliderB);



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// 追加されたコライダーコンポネートのポインタの配列
			std::vector<CCC::Interfaces::IComponent*> m_Colliders;
		};
	}
}

/**
 * @file   ColliderComponentBase.h
 *
 * @brief  コライダー基底コンポネートクラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2026/02/05
 *
 * コライダーの基底クラスとなるコンポネートクラス
 *
 * 2026/02/05
 * 作成
 */

// 多重インクルードガード
#pragma once

// 基底クラス
#include <CCC/Components/ComponentBase.h>

// コライダーの種類
#include "ColliderType.h"

namespace CCC
{
	// 前方宣言
	namespace Components
	{
		class Transform;
	}

	namespace Bases
	{
		/// <summary>
		///  コライダー基底コンポネートクラス
		/// </summary>
		class ColliderComponentBase :
			public CCC::Bases::ComponentBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="p_Owner">オーナーへのポインタ</param>
			/// <param name="p_Transform">トランスフォームコンポネートへのポインタ</param>
			/// <param name="isStatic">静的なコライダーか？</param>
			/// <param name="type">コライダーの種類</param>
			ColliderComponentBase(
				CCC::Interfaces::IObject*           p_Owner,
				CCC::Components::Transform*         p_Transform,
				bool                                isStatic,
				const CCC::Colliders::ColliderType& type
			);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~ColliderComponentBase() noexcept;





			// ---------------------------------------------------------------------- //
			// 取得関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// トランスフォームコンポネートへのポインタの取得
			/// </summary>
			CCC::Components::Transform* GetTransform();

			/// <summary>
			/// 静的なコライダーか？
			/// </summary>
			bool IsStatic() const;

			/// <summary>
			/// コライダーの種類を取得
			/// </summary>
			CCC::Colliders::ColliderType GetType() const;



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			CCC::Components::Transform*  mp_Transform;	// トランスフォーム
			bool                         m_IsStatic;	// 静的なコライダーか？
			CCC::Colliders::ColliderType m_Type;		// コライダーの種類
		};
	}
}

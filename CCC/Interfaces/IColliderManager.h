/**
 * @file   IColliderManager.h
 *
 * @brief  コライダー管理インターフェースクラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2026/02/05
 *
 * コライダー管理の抽象クラス
 * コライダー管理クラスはこのインターフェースから実装がなされていなければならない
 *
 * 2026/02/05
 * 作成
 */

// 多重インクルードガード
#pragma once

// インターフェース
#include <CCC/Interfaces/IComponent.h>

namespace CCC::Interfaces
{
	/// <summary>
	/// コライダー管理のインターフェースクラス
	/// </summary>
	class IColliderManager
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~IColliderManager() noexcept = default;

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// コライダーコンポーネントの追加
		/// </summary>
		virtual void AddCollider(IComponent* p_ColliderComponent) = 0;
	};
}

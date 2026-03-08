/**
 * @file   KnockbackState.h
 *
 * @brief  キャラクタークラスの「ぶっ飛ばされた」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/17
 * 作成
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴う、アドレスの変更
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

// 多重インクルードガード
#pragma once

// 基底クラス
#include "StateBase.h"

namespace CCC::States
{
	/// <summary>
	/// 「ぶっ飛ばされた」状態
	/// </summary>
	class KnockbackState final :
		public CCC::Bases::StateBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		KnockbackState(CCC::Bases::CharacterBase* p_Owner);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~KnockbackState();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="elapsedTime">経過時間</param>
		void Update(float elapsedTime) override final;
	};
}

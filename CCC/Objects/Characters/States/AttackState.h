/**
 * @file   AttackState.h
 *
 * @brief  キャラクタークラスの「攻撃」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/05
 * アニメーション機能の問題で
 * アニメーションが終了しても終了が取れない問題の一時的な解決案として
 * １.3秒たったら強制的に切り替えるように機能を改修
 *
 * 2026/01/17
 * コメントを追加
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴うアドレスの変更
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
	/// 「攻撃」状態
	/// </summary>
	class AttackState final :
		public CCC::Bases::StateBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		AttackState(CCC::Bases::CharacterBase* p_Owner);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~AttackState();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="elapsedTime">経過時間</param>
		void Update(float elapsedTime) override final;



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		// 経過時間の累計
		float m_Time;
	};
}

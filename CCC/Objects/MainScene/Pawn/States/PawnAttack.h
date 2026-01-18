/**
 * @file   PawnAttack.h
 *
 * @brief  ポーンオブジェクトクラスの「攻撃」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/17
 *
 * 2026/01/05
 * アニメーション機能の問題で
 * アニメーションが終了しても終了が取れない問題の一時的な解決案として
 * １.3秒たったら強制的に切り替えるように機能を改修
 *
 * 2026/01/17
 * コメントを追加
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include "PawnState.h"

/// <summary>
/// 「攻撃」状態
/// </summary>
class PawnAttack final:
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnAttack(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnAttack();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime"></param>
	void Update(float elapsedTime) override final;

private:
	// 経過時間の累計
	float m_Time;
};


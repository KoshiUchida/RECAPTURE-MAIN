/**
 * @file   PawnKnockback.h
 *
 * @brief  ポーンオブジェクトクラスの「ぶっ飛ばされた」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/17
 *
 * 2026/01/17
 * 作成
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include "PawnState.h"

/// <summary>
/// 「ぶっ飛ばされた」状態
/// </summary>
class PawnKnockback final :
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnKnockback(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnKnockback();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override final;
};


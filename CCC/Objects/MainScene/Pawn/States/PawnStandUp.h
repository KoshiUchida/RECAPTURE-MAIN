/**
 * @file   PawnStandUp.h
 *
 * @brief  ポーンオブジェクトクラスの「立ち上がる」状態のヘッダファイル
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
/// 「立ち上がる」状態
/// </summary>
class PawnStandUp final :
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnStandUp(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnStandUp();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override final;
};


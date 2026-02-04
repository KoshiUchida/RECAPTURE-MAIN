/**
 * @file   PawnBlock.h
 *
 * @brief  ポーンオブジェクトクラスの「防御」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/28
 * この状態で敵の攻撃を受けても多少のぶっ飛びは効かない
 * 
 * 2026/01/28
 * 作成
 */

 // 多重インクルードガード
#pragma once

// 親クラス
#include "PawnState.h"

/// <summary>
/// 「防御」状態
/// </summary>
class PawnBlock final :
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnBlock(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnBlock();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override final;

private:
	// 外力を受け止めた回数
	int m_OutSideCount;
};

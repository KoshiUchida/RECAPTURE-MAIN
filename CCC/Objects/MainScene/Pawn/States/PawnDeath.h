/**
 * @file   PawnDeath.h
 *
 * @brief  ポーンオブジェクトクラスの「死亡」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/25
 *
 * 2026/01/25
 * 作成
 */

 // 多重インクルードガード
#pragma once

// 親クラス
#include "PawnState.h"

class PawnDeath final :
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnDeath(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnDeath();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override final;

private:
	// 経過時間の累計
	float m_Time;

	// 停止
	bool m_Stop;
};

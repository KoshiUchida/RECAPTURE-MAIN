/**
 * @file   PawnIdle.h
 *
 * @brief  ポーンオブジェクトクラスの待機ステートのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/28
 *
 * 2026/01/28
 * コメントを追加
 * 入力管理クラスへのポインタのメンバ変数があったが不要になったため削除
 */ 

// 多重インクルードガード
#pragma once

// 親クラス
#include "PawnState.h"

/// <summary>
/// 待機するステート
/// </summary>
class PawnIdle final:
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnIdle(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnIdle();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過処理</param>
	void Update(float elapsedTime) override final;


private:
	// 内部関数

	/// <summary>
	/// 状態を変えるかどうかを判断する
	/// </summary>
	void RequestStateTransition();
};


/**
 * @file   PawnMove.h
 *
 * @brief  ポーンオブジェクトクラスの移動ステートのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/17
 * 
 * 2026/01/17
 * コメントを追加
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include "PawnState.h"

// 前方宣言
namespace CCC::Managers
{
	class InputManager;
}

/// <summary>
/// 移動するステート
/// </summary>
class PawnMove final:
	public PawnState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnMove(Pawn* p_Owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PawnMove();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override final;

private:
	// 内部関数

	/// <summary>
	/// 状態を変えるかどうかを判断する
	/// </summary>
	void RequestStateTransition();

private:
	// 入力管理クラスへのポインタ
	CCC::Managers::InputManager* mp_InputManager;

	// 止まる範囲まで到達したか
	bool m_HaveReached;
};


/**
 * @file   PawnAttack.cpp
 *
 * @brief  ポーンオブジェクトクラスの「攻撃」状態のソースファイル
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

// プリコンパイル済みヘッダー	
#include "pch.h"

// クラス定義元
#include "PawnAttack.h"

// このステートマシンを持つクラス
#include "../Pawn.h"

PawnAttack::PawnAttack(Pawn* p_Owner) :
	PawnState(p_Owner),
	m_Time   (0.0f)
{
	// アニメーションを「Paladin_Slash」に変更するようリクエスト
	this->RequestAnimationChange("Paladin_Slash", 0.3f);
	this->GetOwner()->ResetAnimationSpeed();
}

PawnAttack::~PawnAttack() = default;

void PawnAttack::Update(float elapsedTime)
{
	m_Time += elapsedTime;


	// アニメーションが終了するまたは、1.3秒の経過で「待機」状態に遷移するようリクエスト
	if (this->GetOwner()->EndCurrentAnimation() || m_Time > 1.3f)
	{
		this->GetOwner()->RequestStateChange("Idle");
	}
}

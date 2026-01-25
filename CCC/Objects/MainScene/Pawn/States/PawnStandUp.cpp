/**
 * @file   PawnStandUp.cpp
 *
 * @brief  ポーンオブジェクトクラスの「立ち上がる」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/17
 *
 * 2026/01/17
 * 作成
 */

 // プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "PawnStandUp.h"

// このステートマシンを持つクラス
#include "../Pawn.h"

// コンポネート
#include <CCC/Objects/PawnCollider.h>

PawnStandUp::PawnStandUp(Pawn* p_Owner) :
	PawnState(p_Owner)
{
	this->RequestAnimationChange("Paladin_StandUp", 0.0f);
	this->GetOwner()->ResetAnimationSpeed();
}

PawnStandUp::~PawnStandUp()
{
	this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->SetActive(true);
}

void PawnStandUp::Update(float elapsedTime)
{
	// 警告回避
	elapsedTime;

	// アニメーションが終了したら「待機」状態に遷移
	if (this->GetOwner()->EndCurrentAnimation())
	{
		this->GetOwner()->RequestStateChange("Idle");
	}
}

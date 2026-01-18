/**
 * @file   PawnKnockback.cpp
 *
 * @brief  ポーンオブジェクトクラスの「ぶっ飛ばされた」状態のソースファイル
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
#include "PawnKnockback.h"

// このステートマシンを持つクラス
#include "../Pawn.h"

// コンポネート
#include <CCC/Objects/PawnCollider.h>

PawnKnockback::PawnKnockback(Pawn* p_Owner) :
	PawnState(p_Owner)
{
	this->RequestAnimationChange("Paladin_FallingBackDeath", 0.1f);
	this->GetOwner()->ResetAnimationSpeed();
	
	this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->SetActive(false);
}

PawnKnockback::~PawnKnockback()
{
	this->GetOwner()->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
}

void PawnKnockback::Update(float elapsedTime)
{
	// 警告回避
	elapsedTime;

	//
	this->GetOwner()->SetVelocity(this->GetOwner()->GetVelocity() * 0.99f);

	// アニメーションが終了したら「待機」状態に遷移
	if (this->GetOwner()->EndCurrentAnimation())
	{
		this->GetOwner()->RequestStateChange("StandUp");
	}
}

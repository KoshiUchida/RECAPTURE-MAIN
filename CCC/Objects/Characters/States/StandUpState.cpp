/**
 * @file   StandUpState.cpp
 *
 * @brief  キャラクタークラスの「立ち上がる」ステートのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/17
 * 作成
 * 
 * 2026/02/27
 * キャラクタークラスの実装に伴い、アドレスを変更
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

 // プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "StandUpState.h"

// このステートマシンを持つクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/AnimationType.h>

// コンポネート
#include <CCC/Objects/PawnCollider.h>

namespace CCC::States
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	StandUpState::StandUpState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();
		owner->GetAnimationState()->RequestAnimationChange(owner->GetAnimationTypeName(CCC::AnimationType::StandUp), 0.0f);
		owner->GetAnimationState()->ResetAnimationSpeed();
	}

	StandUpState::~StandUpState()
	{
		this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->SetActive(true);
	}

	void StandUpState::Update(float)
	{
		// アニメーションが終了したら「待機」状態に遷移
		if (this->GetOwner()->GetAnimationState()->EndCurrentAnimation())
			this->GetOwner()->RequestStateChange(CCC::StateType::Idle);
	}
}

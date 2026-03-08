/**
 * @file   KnockbackState.cpp
 *
 * @brief  キャラクタークラスの「ぶっ飛ばされた」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/17
 * 作成
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴う、アドレスの変更
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

 // プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "KnockbackState.h"

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

	KnockbackState::KnockbackState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		owner->GetAnimationState()->RequestAnimationChange(owner->GetAnimationTypeName(CCC::AnimationType::FallingBack), 0.1f);
		owner->GetAnimationState()->ResetAnimationSpeed();

		owner->GetComponent<CCC::Components::PawnCollider>("Collider")->SetActive(false);
	}

	KnockbackState::~KnockbackState()
	{
		this->GetOwner()->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
	}

	void KnockbackState::Update(float)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		// 摩擦力で少しづつ減速させる
		owner->SetVelocity(owner->GetVelocity() * 0.99f);

		// アニメーションが終了したら「待機」状態に遷移
		if (owner->GetAnimationState()->EndCurrentAnimation())
		{
			// 「死亡」行き
			if (owner->GetHP() <= 0.0f)
			{
				owner->RequestStateChange(CCC::StateType::Death);
				return;
			}

			// 「立ち上がり」行き
			owner->RequestStateChange(CCC::StateType::StandUp);
		}
	}
}

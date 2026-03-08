/**
 * @file   DeathState.cpp
 *
 * @brief  キャラクタークラスの「死亡」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/25
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
#include "DeathState.h"

// このステートを所持するクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/AnimationType.h>

// コンポネートクラス
#include <CCC/Objects/PawnCollider.h>

namespace CCC::States
{
	DeathState::DeathState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner),
		m_Time(0.0f),
		m_Stop(false)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		owner->GetAnimationState()->RequestAnimationChange(owner->GetAnimationTypeName(CCC::AnimationType::Death), 0.3f);
		owner->GetComponent<CCC::Components::PawnCollider>("Collider")->SetActive(false);
		owner->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
	}

	DeathState::~DeathState() = default;

	void DeathState::Update(float elapsedTime)
	{
		if (m_Stop) return;

		CCC::Animations::AnimationStateBace* animation         = this->GetOwner()->GetAnimationState();
		float                                animationDuration = static_cast<float>(animation->GetCurrentAnimationDuration());

		m_Time += elapsedTime;

		if (m_Time >= animationDuration)
		{
			m_Time = animationDuration;
			m_Stop = true;
			animation->SetAnimationSpeed(0.0f);
			animation->SetAnimationTime(m_Time);
		}
	}
}

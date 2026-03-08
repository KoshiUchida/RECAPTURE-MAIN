/**
 * @file   ChaseState.cpp
 *
 * @brief  パラディンキャラクターの「追跡」ステートのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/03/04
 * 作成
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 * 
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "ChaseState.h"

// コンポネートクラス
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

// このステートマシンを持つクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/AnimationType.h>
#include "../Paladin/PaladinParameter.h"

namespace CCC::States
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	ChaseState::ChaseState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner)
	{
	}

	ChaseState::~ChaseState() = default;

	void ChaseState::Update(float elapsedTime)
	{
		std::string animation;

		CCC::Bases::CharacterBase* owner = this->GetOwner();
		CCC::Bases::CharacterBase* target = owner->GetTarget();
		CCC::Components::Transform* targetTransform = target->GetTransform();

		DirectX::SimpleMath::Vector3    targetPosition = targetTransform->GetPosition();
		DirectX::SimpleMath::Quaternion targetRotation = targetTransform->GetQuaternion();
		DirectX::SimpleMath::Vector3    worldOffset =
			DirectX::SimpleMath::Vector3::Transform(owner->GetOffset(), targetRotation);

		float distanceToTarget = ((targetPosition + worldOffset) - owner->GetTransform()->GetPosition()).Length();

		if (distanceToTarget > CCC::PaladinParamerter::RUN_RADIUS)
		{
			owner->SetRunning(true);
			animation = owner->GetAnimationTypeName(CCC::AnimationType::Run);
		}
		else
		{
			owner->SetRunning(false);
			animation = owner->GetAnimationTypeName(CCC::AnimationType::Walk);
		}


		// 隊列位置へ到着するように移動
		// 陣形安定度のパーセンテージによって、ワンダーの影響度が入る
		float stability = owner->GetFormationStability();
		DirectX::SimpleMath::Vector3 steering
			= owner->Arrive(owner->Pursuit(targetPosition + worldOffset, target->GetVelocity()) * stability + owner->Wander() * (1.0f - stability));

		owner->AddVelocity(steering * elapsedTime);

		// アニメーションの変更リクエスト
		owner->GetAnimationState()->RequestAnimationChange(animation, 0.5f);

		// 状態遷移
		this->RequestStateTransition();
	}



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //
	void ChaseState::RequestStateTransition()
	{
		// ---------------------------------------------------------------------- //
		// 状態遷移
		// ---------------------------------------------------------------------- //
		using CCC::StateType;

		CCC::Bases::CharacterBase* owner = this->GetOwner();

		// 「死亡」行き
		if (owner->GetHP() <= 0.0f)
		{
			owner->RequestStateChange(StateType::Death);
			return;
		}

		// 「ノックバック」行き
		// 外力があったら吹っ飛ぶ
		DirectX::SimpleMath::Vector3 outSide = owner->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
		if (outSide != DirectX::SimpleMath::Vector3::Zero)
		{
			owner->SetVelocity(outSide);
			owner->RequestStateChange(StateType::Knockback);
		}
	}
}

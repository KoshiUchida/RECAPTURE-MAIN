/**
 * @file   PawnMove.cpp
 *
 * @brief  ポーンオブジェクトクラスの移動ステートのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/17
 *
 * 2026/01/17
 * コメントを追加
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "PawnMove.h"

// 管理クラス
#include <CCC/Managers/InputManager.h>

// コンポネートクラス
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

// このステートマシンを持つクラス
#include "../Pawn.h"
#include "../PawnParameter.h"

PawnMove::PawnMove(Pawn* p_Owner) :
	PawnState(p_Owner),
	mp_InputManager(CCC::Managers::InputManager::GetInstance()),
	m_HaveReached(false)
{
}

PawnMove::~PawnMove() = default;

void PawnMove::Update(float elapsedTime)
{
	std::string animation;

	DirectX::SimpleMath::Vector3    targetPosition = this->GetTarget()->GetPosition();
	DirectX::SimpleMath::Quaternion targetRotation = this->GetTarget()->GetTransform()->GetQuaternion();
	DirectX::SimpleMath::Vector3    worldOffset =
		DirectX::SimpleMath::Vector3::Transform(this->GetOffset(), targetRotation);

	float distanceToTarget = ((targetPosition + worldOffset) - this->GetPosition()).Length();

	if (distanceToTarget > PawnParameter::RUN_RADIUS)
	{
		this->GetOwner()->SetRunning(true);
		animation = "Paladin_Run";
	}
	else
	{
		this->GetOwner()->SetRunning(false);
		animation = "Paladin_Walk";
	}

	// 目的地に到達したか
	m_HaveReached = !this->GetTarget() || distanceToTarget < PawnParameter::STOP_RADIUS;


	// 隊列位置へ到着するように移動
	// 陣形安定度のパーセンテージによって、ワンダーの影響度が入る
	float stability = this->GetOwner()->GetFormationStability();
	DirectX::SimpleMath::Vector3 steering
		= this->GetOwner()->Arrive(this->GetOwner()->Pursuit(targetPosition + worldOffset, this->GetTarget()->GetVelocity()) * stability + this->GetOwner()->Wander() * (1.0f - stability));

	this->GetOwner()->AddVelocity(steering * elapsedTime);

	// アニメーションの変更リクエスト
	this->RequestAnimationChange(animation, 0.5f);

	// 状態遷移
	this->RequestStateTransition();
}

void PawnMove::RequestStateTransition()
{
	// ---------------------------------------------------------------------- //
	// 状態遷移
	// ---------------------------------------------------------------------- //

	// 「ノックバック」行き
	// 外力があったら吹っ飛ぶ
	DirectX::SimpleMath::Vector3 outSide = this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
	if (outSide != DirectX::SimpleMath::Vector3::Zero)
	{
		this->GetOwner()->SetVelocity(outSide);
		this->GetOwner()->RequestStateChange("Knockback");
	}

	// 「攻撃」行き
	if (this->GetOwner()->GetTarget()->IsAttacking())
	{
		this->GetOwner()->RequestStateChange("Attack");
		return;
	}

	// 「待機」行き
	if (m_HaveReached)
	{
		this->GetOwner()->RequestStateChange("Idle");
		return;
	}
}

#include "pch.h"
#include "PawnMove.h"

#include <CCC/Managers/InputManager.h>

#include <CCC/Components/Transform.h>

#include "../Pawn.h"
#include "../PawnParameter.h"

PawnMove::PawnMove(Pawn* p_Owner) :
	PawnState(p_Owner),
	mp_InputManager(CCC::Managers::InputManager::GetInstance())
{
}

PawnMove::~PawnMove()
{
}

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


	// 隊列位置へ到着するように移動
	// 陣形安定度のパーセンテージによって、ワンダーの影響度が入る
	float stability = this->GetOwner()->GetFormationStability();
	DirectX::SimpleMath::Vector3 steering
		= this->GetOwner()->Arrive(this->GetOwner()->Pursuit(targetPosition + worldOffset, this->GetTarget()->GetVelocity()) * stability + this->GetOwner()->Wander() * (1.0f - stability));

	this->GetOwner()->AddVelocity(steering * elapsedTime);

	this->RequestAnimationChange(animation, 0.5f);


	if (this->GetOwner()->GetTarget()->IsAttacking())
	{
		this->GetOwner()->RequestStateChange("Attack");
		return;
	}

	if (!this->GetTarget() || distanceToTarget < PawnParameter::STOP_RADIUS)
	{
		this->GetOwner()->RequestStateChange("Idle");
		return;
	}
}

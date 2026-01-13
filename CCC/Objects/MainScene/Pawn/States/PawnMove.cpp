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

	DirectX::SimpleMath::Vector3    targetPosition = GetTarget()->GetPosition();
	DirectX::SimpleMath::Quaternion targetRotation = GetTarget()->GetTransform()->GetQuaternion();
	DirectX::SimpleMath::Vector3    worldOffset =
		DirectX::SimpleMath::Vector3::Transform(GetOffset(), targetRotation);

	float distanceToTarget = ((targetPosition + worldOffset) - GetPosition()).Length();

	if (distanceToTarget > PawnParameter::RUN_RADIUS)
	{
		GetOwner()->SetRunning(true);
		animation = "Paladin_Run";
	}
	else
	{
		GetOwner()->SetRunning(false);
		animation = "Paladin_Walk";
	}


	// 隊列位置へ到着するように移動
	// 陣形安定度のパーセンテージによって、ワンダーの影響度が入る
	float stability = GetOwner()->GetFormationStability();
	DirectX::SimpleMath::Vector3 steering
		= GetOwner()->Arrive(GetOwner()->Pursuit(targetPosition + worldOffset, GetTarget()->GetVelocity()) * stability + GetOwner()->Wander() * (1.0f - stability));

	GetOwner()->AddVelocity(steering * elapsedTime);

	RequestAnimationChange(animation, 0.5f);


	if (!GetTarget() || distanceToTarget < PawnParameter::STOP_RADIUS)
	{
		GetOwner()->RequestStateChange("Idle");
		return;
	}

	if (mp_InputManager->GetInputAs<bool>("Attack"))
	{
		GetOwner()->RequestStateChange("Attack");
		return;
	}
}

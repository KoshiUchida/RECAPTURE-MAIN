#include "pch.h"
#include "PawnIdle.h"

#include <CCC/Components/Transform.h>
#include <CCC/Managers/InputManager.h>

#include "../Pawn.h"
#include "../PawnParameter.h"

PawnIdle::PawnIdle(Pawn* p_Owner) :
	PawnState(p_Owner),
	mp_InputManager(CCC::Managers::InputManager::GetInstance())
{
	GetOwner()->ResetAnimationSpeed();
	RequestAnimationChange("Paladin_Idle", 0.3f);

	SetVelocity(DirectX::SimpleMath::Vector3::Zero);
}

PawnIdle::~PawnIdle() = default;

void PawnIdle::Update(float elapsedTime)
{
	// Œx‰ñ”ð
	elapsedTime;

	SetRotate(GetTarget()->GetTransform()->GetRotate());


	DirectX::SimpleMath::Vector3    targetPosition = GetTarget()->GetPosition();
	DirectX::SimpleMath::Quaternion targetRotation = GetTarget()->GetTransform()->GetQuaternion();
	DirectX::SimpleMath::Vector3    worldOffset =
		DirectX::SimpleMath::Vector3::Transform(GetOffset(), targetRotation);

	float distanceToTarget = ((targetPosition + worldOffset) - GetPosition()).Length();

	if (distanceToTarget > PawnParameter::STOP_RADIUS)
	{
		GetOwner()->RequestStateChange("Move");

		return;
	}

	if (mp_InputManager->GetInputAs<bool>("Attack"))
	{
		GetOwner()->RequestStateChange("Attack");
		return;
	}
}

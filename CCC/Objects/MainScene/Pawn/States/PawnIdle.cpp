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
	this->GetOwner()->ResetAnimationSpeed();
	this->RequestAnimationChange("Paladin_Idle", 0.3f);

	this->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
}

PawnIdle::~PawnIdle() = default;

void PawnIdle::Update(float elapsedTime)
{
	// Œx‰ñ”ð
	elapsedTime;

	this->SetRotate(GetTarget()->GetTransform()->GetRotate());


	DirectX::SimpleMath::Vector3    targetPosition = this->GetTarget()->GetPosition();
	DirectX::SimpleMath::Quaternion targetRotation = this->GetTarget()->GetTransform()->GetQuaternion();
	DirectX::SimpleMath::Vector3    worldOffset =
		DirectX::SimpleMath::Vector3::Transform(this->GetOffset(), targetRotation);

	float distanceToTarget = ((targetPosition + worldOffset) - this->GetPosition()).Length();

	if (this->GetOwner()->GetTarget()->IsAttacking())
	{
		this->GetOwner()->RequestStateChange("Attack");
		return;
	}

	if (distanceToTarget > PawnParameter::STOP_RADIUS)
	{
		this->GetOwner()->RequestStateChange("Move");

		return;
	}

}

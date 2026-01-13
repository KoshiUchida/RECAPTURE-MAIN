#include "pch.h"
#include "PawnState.h"

#include "../Pawn.h"

PawnState::PawnState(Pawn* p_Owner) :
	mp_Owner(p_Owner)
{
}

PawnState::~PawnState() = default;

Pawn* PawnState::GetOwner()
{
	return mp_Owner;
}

void PawnState::RequestAnimationChange(const std::string& animation, float blendTime)
{
	mp_Owner->RequestAnimationChange(animation, blendTime);
}

void PawnState::SetVelocity(const DirectX::SimpleMath::Vector3& vel)
{
	mp_Owner->SetVelocity(vel);
}

void PawnState::SetRotate(const DirectX::SimpleMath::Vector3& rot)
{
	mp_Owner->SetRotate(rot);
}

DirectX::SimpleMath::Vector3 PawnState::GetPosition() const
{
	return mp_Owner->GetPosition();
}

CCC::Bases::PawnBase* PawnState::GetTarget()
{
	return mp_Owner->GetTarget();
}

DirectX::SimpleMath::Vector3 PawnState::GetOffset() const
{
	return mp_Owner->GetOffset();
}

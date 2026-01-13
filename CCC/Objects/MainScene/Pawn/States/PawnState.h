#pragma once
#include <CCC/Interfaces/IState.h>

#include <CCC/Objects/PawnBase.h>

class Pawn;

class PawnState :
	public CCC::Interfaces::IState
{
public:
	PawnState(Pawn* p_Owner);
	virtual ~PawnState();
	virtual void Update(float elapsedTime) = 0;
	Pawn* GetOwner();

	void RequestAnimationChange(const std::string& animation, float blendTime);

	void SetVelocity(const DirectX::SimpleMath::Vector3& vel);

	void SetRotate(const DirectX::SimpleMath::Vector3& rot);

	DirectX::SimpleMath::Vector3 GetPosition() const;

	CCC::Bases::PawnBase* GetTarget();

	DirectX::SimpleMath::Vector3 GetOffset() const;

private:
	Pawn* mp_Owner;
};


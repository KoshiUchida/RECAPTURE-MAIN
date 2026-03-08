#include "pch.h"
#include "Pawn.h"

#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/Paladin/PaladinParameter.h>

Pawn::Pawn(const TeamID& teamID, PawnManager* p_PawnManager) :
	SpikemanCharacter(teamID, p_PawnManager)
{
}

Pawn::~Pawn() = default;

void Pawn::Start()
{
}

void Pawn::Process(float)
{
	using CCC::StateType;

	StateType         thisState   = this->GetState();
	CCC::Bases::CharacterBase* leader      = this->GetTarget();
	StateType         leaderState = leader->GetState();

	switch (thisState)
	{
	case CCC::StateType::Idle:
		// リーダーの行動に合わせる
		if (leaderState == StateType::Block)
			this->RequestStateChange(StateType::Block);
		if (leaderState == StateType::Attack)
			this->RequestStateChange(StateType::Attack);

		// リーダーが遠いと自身の陣形での位置に向けて移動する
		if (this->GetDiffToTarget() > CCC::PaladinParamerter::STOP_RADIUS)
			this->RequestStateChange(StateType::Chase);
		break;

	case CCC::StateType::Chase:

		// 陣形の定位置に着いたら待機する
		if (this->GetDiffToTarget() <= CCC::PaladinParamerter::STOP_RADIUS)
			this->RequestStateChange(StateType::Idle);
		break;

	case CCC::StateType::Block:
		// リーダーが防御を解除したら自分も解除する
		if (thisState == StateType::Block && leaderState != StateType::Block)
			this->RequestStateChange(StateType::Idle);
		break;

	case CCC::StateType::Move:
	case CCC::StateType::Attack:
	case CCC::StateType::Knockback:
	case CCC::StateType::StandUp:
	case CCC::StateType::Death:
		// 何もしない
		break;
	case CCC::StateType::None:
	default:
		// エラー
		break;
	}
}

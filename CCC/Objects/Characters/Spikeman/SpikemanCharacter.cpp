
// プリコンパイル済みヘッダーファイル
#include "pch.h"

// クラス定義元
#include "SpikemanCharacter.h"

// コンポネートクラス
#include <CCC/Components/Transform.h>

// アニメーションの種別
#include <CCC/Objects/Characters/AnimationType.h>



// ---------------------------------------------------------------------- //
// パブリック関数
// ---------------------------------------------------------------------- //

SpikemanCharacter::SpikemanCharacter(const TeamID& teamID, PawnManager* p_PawnManager) :
	CharacterBase(teamID, p_PawnManager)
{
}

SpikemanCharacter::~SpikemanCharacter() = default;

void SpikemanCharacter::InitializeAnimationState()
{
	// サイズを1.7倍に設定
	this->GetTransform()->SetScale(1.7f);

	this->CreateAnimationState("Spikeman", "Spikeman_Idle");

	CCC::Animations::AnimationStateBace* animation = this->GetAnimationState();

	animation->AddAnimationPlayer("Spikeman_Idle");
	animation->AddAnimationPlayer("Spikeman_Walk");
	animation->AddAnimationPlayer("Spikeman_Run");
	animation->AddAnimationPlayer("Spikeman_Attack");
	animation->AddAnimationPlayer("Spikeman_FallingBack");
	animation->AddAnimationPlayer("Spikeman_StandUp");
	animation->AddAnimationPlayer("Spikeman_Death");
	animation->AddAnimationPlayer("Spikeman_Block");

	this->AddAnimationTypeName(CCC::AnimationType::Idle       , "Spikeman_Idle");
	this->AddAnimationTypeName(CCC::AnimationType::Walk       , "Spikeman_Walk");
	this->AddAnimationTypeName(CCC::AnimationType::Run        , "Spikeman_Run");
	this->AddAnimationTypeName(CCC::AnimationType::Attack     , "Spikeman_Attack");
	this->AddAnimationTypeName(CCC::AnimationType::FallingBack, "Spikeman_FallingBack");
	this->AddAnimationTypeName(CCC::AnimationType::StandUp    , "Spikeman_StandUp");
	this->AddAnimationTypeName(CCC::AnimationType::Death      , "Spikeman_Death");
	this->AddAnimationTypeName(CCC::AnimationType::Block      , "Spikeman_Block");
}

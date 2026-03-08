/**
 * @file   PaladinCharacter.cpp
 *
 * @brief  パラディンキャラクタークラスのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/02/26
 * 作成
 *
 * 2026/02/27
 * Pawnクラスの設計を参考に大幅の拡張
 * 
 * 2026/03/08
 * 共通化のため、大幅にソースコードを減らした
 * アニメーションのリソース登録名を保存するように追加
 */

// プリコンパイル済みヘッダーファイル
#include "pch.h"

// クラス定義元
#include "PaladinCharacter.h"

// コンポネートクラス
#include <CCC/Components/Transform.h>

// アニメーションの種別
#include <CCC/Objects/Characters/AnimationType.h>



// ---------------------------------------------------------------------- //
// パブリック関数
// ---------------------------------------------------------------------- //

PaladinCharacter::PaladinCharacter(const TeamID& teamID, PawnManager* p_PawnManager) :
	CharacterBase(teamID, p_PawnManager)
{
}

PaladinCharacter::~PaladinCharacter() = default;



// ---------------------------------------------------------------------- //
// メンバ関数
// ---------------------------------------------------------------------- //

void PaladinCharacter::InitializeAnimationState()
{
	// サイズを0.02倍に設定
	this->GetTransform()->SetScale(0.02);

	this->CreateAnimationState("Paladin", "Paladin_Idle");

	CCC::Animations::AnimationStateBace* animation = this->GetAnimationState();

	animation->AddAnimationPlayer("Paladin_Idle");
	animation->AddAnimationPlayer("Paladin_Walk");
	animation->AddAnimationPlayer("Paladin_Run");
	animation->AddAnimationPlayer("Paladin_Slash");
	animation->AddAnimationPlayer("Paladin_FallingBackDeath");
	animation->AddAnimationPlayer("Paladin_StandUp");
	animation->AddAnimationPlayer("Paladin_Death");
	animation->AddAnimationPlayer("Paladin_BlockIdle");

	this->AddAnimationTypeName(CCC::AnimationType::Idle       , "Paladin_Idle");
	this->AddAnimationTypeName(CCC::AnimationType::Walk       , "Paladin_Walk");
	this->AddAnimationTypeName(CCC::AnimationType::Run        , "Paladin_Run");
	this->AddAnimationTypeName(CCC::AnimationType::Attack     , "Paladin_Slash");
	this->AddAnimationTypeName(CCC::AnimationType::FallingBack, "Paladin_FallingBackDeath");
	this->AddAnimationTypeName(CCC::AnimationType::StandUp    , "Paladin_StandUp");
	this->AddAnimationTypeName(CCC::AnimationType::Death      , "Paladin_Death");
	this->AddAnimationTypeName(CCC::AnimationType::Block      , "Paladin_BlockIdle");
}

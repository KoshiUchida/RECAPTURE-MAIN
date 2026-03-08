/**
 * @file   MoveState.cpp
 *
 * @brief  キャラクターの「移動」ステートクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/03/08
 * すでにあるベロシティに沿って移動するだけのステート
 *
 * 2026/03/04
 * 作成
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

// プリコンパイル済みヘッダーファイル
#include "pch.h"

// クラス定義元
#include "MoveState.h"

// コンポネートクラス
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

// このステートマシンを持つクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/AnimationType.h>


namespace CCC::States
{
	MoveState::MoveState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner),
		m_IsRunning     (p_Owner->IsRunning())
	{
		this->ChangeAnimation();
	}

	MoveState::~MoveState() = default;

	void CCC::States::MoveState::Update(float)
	{
		using CCC::StateType;

		CCC::Bases::CharacterBase* owner = this->GetOwner();



		// ---------------------------------------------------------------------- //
		// 走っているかを確認する
		// ---------------------------------------------------------------------- //
		if (m_IsRunning != owner->IsRunning())
		{
			m_IsRunning = owner->IsRunning();

			this->ChangeAnimation();
		}



		// ---------------------------------------------------------------------- //
		// 状態遷移
		// ---------------------------------------------------------------------- //

		// 「死亡」行き
		if (owner->GetHP() <= 0.0f)
		{
			owner->RequestStateChange(StateType::Death);
			return;
		}

		// 「ノックバック」行き
		// 外力があったら吹っ飛ぶ
		DirectX::SimpleMath::Vector3 outSide = owner->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
		if (outSide != DirectX::SimpleMath::Vector3::Zero)
		{
			owner->SetVelocity(outSide);
			owner->RequestStateChange(StateType::Knockback);
		}
	}



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //
	void MoveState::ChangeAnimation()
	{
		std::string animation;

		if (m_IsRunning)
			animation = this->GetOwner()->GetAnimationTypeName(CCC::AnimationType::Run);
		else
			animation = this->GetOwner()->GetAnimationTypeName(CCC::AnimationType::Walk);

		this->GetOwner()->GetAnimationState()->RequestAnimationChange(animation, 0.5f);
	}
}

/**
 * @file   IdleState.cpp
 *
 * @brief  ポーンオブジェクトクラスの待機ステートのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/28
 * コメントを追加
 * 入力管理クラスへのポインタのメンバ変数があったが不要になったため削除
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴う、アドレスの変更
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "IdleState.h"

// コンポネート
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

// オーナーのクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/AnimationType.h>

namespace CCC::States
{
	IdleState::IdleState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		owner->GetAnimationState()->ResetAnimationSpeed();
		owner->GetAnimationState()->RequestAnimationChange(owner->GetAnimationTypeName(CCC::AnimationType::Idle), 0.3f);

		owner->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
	}

	IdleState::~IdleState() = default;

	void IdleState::Update(float)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		if (owner->GetTarget())
			// リーダーと向きを合わせる
			owner->GetTransform()->SetRotate(owner->GetTarget()->GetTransform()->GetRotate());

		// 状態遷移
		this->RequestStateTransition();
	}

	void IdleState::RequestStateTransition()
	{
		using CCC::StateType;

		CCC::Bases::CharacterBase* owner = this->GetOwner();

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
			return;
		}
	}
}

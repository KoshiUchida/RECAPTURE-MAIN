/**
 * @file   PawnIdle.cpp
 *
 * @brief  ポーンオブジェクトクラスの待機ステートのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/28
 *
 * 2026/01/28
 * コメントを追加
 * 入力管理クラスへのポインタのメンバ変数があったが不要になったため削除
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "PawnIdle.h"

// コンポネート
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

// オーナーのクラス
#include "../Pawn.h"
#include "../PawnParameter.h"

PawnIdle::PawnIdle(Pawn* p_Owner) :
	PawnState(p_Owner)
{
	this->GetOwner()->ResetAnimationSpeed();
	this->RequestAnimationChange("Paladin_Idle", 0.3f);

	this->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
}

PawnIdle::~PawnIdle() = default;

void PawnIdle::Update(float)
{
	// リーダーと向きを合わせる
	this->SetRotate(this->GetTarget()->GetTransform()->GetRotate());

	// 状態遷移
	this->RequestStateTransition();
}

void PawnIdle::RequestStateTransition()
{
	// 「ノックバック」行き
	// 外力があったら吹っ飛ぶ
	DirectX::SimpleMath::Vector3 outSide = this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
	if (outSide != DirectX::SimpleMath::Vector3::Zero)
	{
		this->GetOwner()->SetVelocity(outSide);
		this->GetOwner()->RequestStateChange("Knockback");
		return;
	}


	// 「攻撃」行き
	if (this->GetOwner()->GetTarget()->IsAttacking())
	{
		this->GetOwner()->RequestStateChange("Attack");
		return;
	}


	// 「移動」行き
	DirectX::SimpleMath::Vector3    targetPosition = this->GetTarget()->GetPosition();
	DirectX::SimpleMath::Quaternion targetRotation = this->GetTarget()->GetTransform()->GetQuaternion();
	DirectX::SimpleMath::Vector3    worldOffset =
		DirectX::SimpleMath::Vector3::Transform(this->GetOffset(), targetRotation);

	float distanceToTarget = ((targetPosition + worldOffset) - this->GetPosition()).Length();
	if (distanceToTarget > PawnParameter::STOP_RADIUS)
	{
		this->GetOwner()->RequestStateChange("Move");

		return;
	}


	// 「防御」行き
	// もし、リーダーが防御してたら自分も防御
	if (this->GetOwner()->GetTarget()->GetCurrentAnimationName() == "Paladin_BlockIdle")
	{
		this->GetOwner()->RequestStateChange("Block");

		return;
	}
}

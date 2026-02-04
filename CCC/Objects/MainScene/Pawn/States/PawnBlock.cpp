/**
 * @file   PawnBlock.cpp
 *
 * @brief  ポーンオブジェクトクラスの「防御」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/28
 * この状態で敵の攻撃を受けても多少のぶっ飛びは効かない
 *
 * 2026/01/28
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "PawnBlock.h"

// 管理クラス
#include <CCC/Managers/GameContextManager.h>

// コンポネート
#include <CCC/Objects/PawnCollider.h>

// このステートマシンを持つクラス
#include "../Pawn.h"
#include "../PawnParameter.h"

PawnBlock::PawnBlock(Pawn* p_Owner) :
	PawnState(p_Owner) ,
	m_OutSideCount(0)
{
	// アニメーションを「Paladin_Block」に変更するようリクエスト
	this->RequestAnimationChange("Paladin_BlockIdle", 0.3f);
	this->GetOwner()->ResetAnimationSpeed();
}

PawnBlock::~PawnBlock() = default;

void PawnBlock::Update(float)
{
	// 「ノックバック」行き
	// 外力がある場合の処理
	DirectX::SimpleMath::Vector3 outSide = this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
	if (outSide != DirectX::SimpleMath::Vector3::Zero)
	{
		m_OutSideCount++;

		// 耐久能力を超える外力だったら「ノックバック」へ
		if (m_OutSideCount > PawnParameter::DEFENCE_COUNT ||
			outSide.Length() > PawnParameter::DEFENCE_POWER)
		{
			this->GetOwner()->SetVelocity(outSide);
			this->GetOwner()->RequestStateChange("Knockback");
			return;
		}

		// コンテキストに防御の成功した回数を足す
		CCC::Managers::GameContextManager* p_gcm = CCC::Managers::GameContextManager::GetInstance();
		p_gcm->SetGameData("BlockCount", p_gcm->GetDataAs<int>("BlockCount") + 1);
	}

	// 「待機」行き
	// もし、リーダーが防御姿勢を崩したら自分も崩す
	if (this->GetOwner()->GetTarget()->GetCurrentAnimationName() != "Paladin_BlockIdle")
	{
		this->GetOwner()->RequestStateChange("Idle");

		return;
	}
}

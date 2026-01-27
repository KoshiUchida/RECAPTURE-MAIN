/**
 * @file   PawnDeath.cpp
 *
 * @brief  ポーンオブジェクトクラスの「死亡」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/25
 *
 * 2026/01/25
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "PawnDeath.h"

// 関係のあるオブジェクト
#include <CCC/Objects/MainScene/Pawn/Pawn.h>
#include <CCC/Objects/PawnCollider.h>

PawnDeath::PawnDeath(Pawn* p_Owner) :
	PawnState(p_Owner),
	m_Time   (0.0f),
	m_Stop (false)
{
	this->GetOwner()->RequestAnimationChange("Paladin_Death", 0.3f);
	this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->SetActive(false);
	this->GetOwner()->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
}

PawnDeath::~PawnDeath() = default;

void PawnDeath::Update(float elapsedTime)
{
	if (m_Stop) return;

	m_Time += elapsedTime;

	if (m_Time >= this->GetOwner()->GetCurrentAnimationDuration())
	{
		m_Time = this->GetOwner()->GetCurrentAnimationDuration();
		m_Stop = true;
		this->GetOwner()->SetAnimationSpeed(0.0f);
		this->GetOwner()->SetAnimationTime(m_Time);
	}
}

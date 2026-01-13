// なぜかアニメーションが終了しても終了が取れない問題の一時的な解決案として
// １秒たったら強制敵に切り替えるように機能を改修

#include "pch.h"
#include "PawnAttack.h"

#include "../Pawn.h"

PawnAttack::PawnAttack(Pawn* p_Owner) :
	PawnState(p_Owner),
	m_Time   (0.0f)
{
	RequestAnimationChange("Paladin_Slash", 0.3f);
	GetOwner()->ResetAnimationSpeed();
}

PawnAttack::~PawnAttack() = default;

void PawnAttack::Update(float elapsedTime)
{
	m_Time += elapsedTime;


	// アニメーションが終了するまたは、1.3秒の経過で「待機」状態に遷移
	if (GetOwner()->EndCurrentAnimation() || m_Time > 1.3f)
	{
		GetOwner()->RequestStateChange("Idle");
	}
}

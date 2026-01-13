// 2026/01/05
// ‚P•bŒo‰ß‚µ‚½‚çŸè‚É‘JˆÚ‚·‚é‚æ‚¤‚É‚µ‚½
#pragma once
#include "PawnState.h"

class PawnAttack :
	public PawnState
{
public:
	PawnAttack(Pawn* p_Owner);
	~PawnAttack();
	void Update(float elapsedTime) override final;

private:
	// Œo‰ßŠÔ‚Ì—İŒv
	float m_Time;
};


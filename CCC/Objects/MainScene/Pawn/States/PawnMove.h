#pragma once
#include "PawnState.h"

namespace CCC::Managers
{
	class InputManager;
}

class PawnMove final:
	public PawnState
{
public:
	PawnMove(Pawn* p_Owner);
	~PawnMove();
	void Update(float elapsedTime) override final;

private:
	CCC::Managers::InputManager* mp_InputManager;
};


#pragma once
#include "PawnState.h"

namespace CCC::Managers
{
	class InputManager;
}

class PawnIdle final:
	public PawnState
{
public:
	PawnIdle(Pawn* p_Owner);
	~PawnIdle();
	void Update(float elapsedTime) override final;

private:
	CCC::Managers::InputManager* mp_InputManager;
};


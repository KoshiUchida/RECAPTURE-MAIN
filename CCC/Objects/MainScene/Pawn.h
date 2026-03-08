#pragma once
#include <CCC/Objects/Characters/Spikeman/SpikemanCharacter.h>

class Pawn final:
	public SpikemanCharacter
{
public:
	Pawn(const TeamID& teamID, PawnManager* p_PawnManager);
	~Pawn();
	void Start() override;
	void Process(float elapsedTime) override;
};

#pragma once
#include <CCC/Objects/ObjectBase.h>

class Floor :
	public CCC::Bases::ObjectBase
{
	public:
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	Floor();
	virtual ~Floor();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize() override;
};


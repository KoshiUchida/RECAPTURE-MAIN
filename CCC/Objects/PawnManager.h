#pragma once
#include <CCC/Objects/ObjectBase.h>

#include "PawnCollider.h"

class PawnManager final :
	public CCC::Bases::ObjectBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	PawnManager();
	virtual ~PawnManager();

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render()override;
	void Finalize() override;

	void AddPawnCollider(CCC::Components::PawnCollider* p_PawnCollider);



	// ---------------------------------------------------------------------- //
	// メンバ変数
	// ---------------------------------------------------------------------- //
private:
	// ポーンコライダーの登録
	std::vector<CCC::Components::PawnCollider*> m_PawnColliders;
};


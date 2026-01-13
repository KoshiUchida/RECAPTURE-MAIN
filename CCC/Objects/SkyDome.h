// 2026/01/07
// ì¬

#pragma once
// Šî’êƒNƒ‰ƒX
#include <CCC/Objects/ObjectBase.h>

namespace CCC::Components
{
	class Transform;
}

class SkyDome final:
	public CCC::Bases::ObjectBase
{
public:
	SkyDome();
	~SkyDome();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize() override;

	void SetTarget(ObjectBase* p_Target);

private:
	CCC::Components::Transform* mp_TargetTransform;
};


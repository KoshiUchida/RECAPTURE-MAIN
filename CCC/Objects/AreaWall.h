// 2026/02/08
// 作成

// 多重インクルードガード
#pragma once

// 基底クラス
#include <CCC/Objects/ObjectBase.h>

namespace CCC::Components
{
	class Transform;
}

/// <summary>
/// エリアウォールオブジェクトクラス
/// </summary>
class AreaWall final :
	public CCC::Bases::ObjectBase
{
public:
	AreaWall(float x, float z, float width, float height);
	~AreaWall();

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize() override;

private:
	CCC::Components::Transform* mp_Transform;
};


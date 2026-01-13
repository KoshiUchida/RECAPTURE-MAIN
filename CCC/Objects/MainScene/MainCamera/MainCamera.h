#pragma once
#include <CCC/Objects/ObjectBase.h>

// 前方宣言
namespace CCC
{
	namespace Components
	{
		class Transform;
		class Camera;
	}

	namespace Managers
	{
		class InputManager;
	}
}

class MainCamera :
	public CCC::Bases::ObjectBase
{
public:
	MainCamera();
	~MainCamera();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize() override;

	void SetTarget(CCC::Interfaces::IObject* p_Target);

private:
	// カメラへのポインタ
	CCC::Components::Transform* mp_Target;

	// 自身のトランスフォーム
	CCC::Components::Transform* mp_Transform;

	// 自身のカメラ
	CCC::Components::Camera* mp_Camera;

	// 入力管理クラスへのポインタ
	CCC::Managers::InputManager* mp_InputManager;

	// 現在の注視点の回転角
	float m_CurrentTargetAngle;

	// 現在の注視点の差分Y座標
	float m_CurrentOffsetY;
};


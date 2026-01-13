// 2026/01/07
// カメラの座標をターゲット依存からワールド座標になった

#include "pch.h"
#include "MainCamera.h"
#include "MainCameraParameter.h"

#include <CCC/Managers/InputManager.h>

#include <CCC/Components/Transform.h>
#include <CCC/Components/Camera.h>


MainCamera::MainCamera() :
	ObjectBase(),
	mp_Target{},
	mp_Transform{},
	mp_Camera{},
	mp_InputManager{ CCC::Managers::InputManager::GetInstance() },
	m_CurrentTargetAngle{ 0.0f },
	m_CurrentOffsetY{ 0.0f }
{
	mp_Transform = AddComponent<CCC::Components::Transform>("Transform", this);
	mp_Camera    = AddComponent<CCC::Components::Camera   >("Camera", this, MainCameraParameter::EyeSpeedRate, MainCameraParameter::TargetSpeedRate);
}

MainCamera::~MainCamera() = default;

void MainCamera::Initialize()
{
	// 初期座標の設定
	mp_Transform->SetPosition(
		DirectX::SimpleMath::Vector3(
			MainCameraParameter::Initialize::PositionX,
			MainCameraParameter::Initialize::PositionY,
			MainCameraParameter::Initialize::PositionZ
		)
	);

	m_CurrentOffsetY = MainCameraParameter::Initialize::TargetOffsetY;
}

void MainCamera::Update(float elapsedTime)
{
	using namespace DirectX;

	// 警告回避
	elapsedTime;


	// カメラ操作を取得する
	float horizontal = mp_InputManager->GetInputAs<float>("CameraHorizontalPercent");

	// 注点の回転角をカメラ操作に応じて変更する
	if (horizontal != 0.0f)
	{
		m_CurrentTargetAngle +=
			DirectX::XMConvertToRadians(MainCameraParameter::HorizontalRotateRate * -horizontal) * elapsedTime;
	}

	// 注視点の回転角から注視点の座標を計算する

	mp_Transform->SetPosition(
		SimpleMath::Vector3(
			std::sin(m_CurrentTargetAngle) * 12.0f,
			mp_Transform->GetPosition().y,
			std::cos(m_CurrentTargetAngle) * 12.0f
		)
	);


	// カメラの縦移動
	float vertical = mp_InputManager->GetInputAs<float>("CameraVerticalPercent");

	if (vertical != 0.0f)
	{
		m_CurrentOffsetY += MainCameraParameter::VerticalRotateRate * vertical * elapsedTime;
		m_CurrentOffsetY = std::clamp(
			m_CurrentOffsetY,
			MainCameraParameter::VerticalDownLimit,
			MainCameraParameter::VerticalUpLimit);
		mp_Transform->SetPositionY(m_CurrentOffsetY);
	}




	// ターゲットがない場合は早期リターン
	if (!mp_Target) return;

	// 注視点の座標を計算
	SimpleMath::Vector3 targetPos =
		mp_Target->GetPosition() + SimpleMath::Vector3(0.0f, 3.0f, 0.0f);
	

	const float distance = 12.0f;
	SimpleMath::Vector3 offset =
		SimpleMath::Vector3(
			std::sinf(m_CurrentTargetAngle) * distance,
			m_CurrentOffsetY,
			std::cosf(m_CurrentTargetAngle) * distance
		);


	// 視点の座標を計算
	SimpleMath::Vector3 eyePos = targetPos + offset;
	


	// カメラコンポネートに座標情報を設定する
	mp_Camera->SetCameraPositions(eyePos, targetPos);
}

void MainCamera::Render()
{
}

void MainCamera::Finalize()
{
}

void MainCamera::SetTarget(CCC::Interfaces::IObject* p_Target)
{
	// 注点になるオブジェクトからトランスフォームコンポネートへのポインタを取得
	mp_Target = static_cast<CCC::Components::Transform*>(
		static_cast<CCC::Bases::ObjectBase*>(p_Target)->GetComponent<CCC::Components::Transform>("Transform")
		);
}

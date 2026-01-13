/**
 * @file   Camera.h
 *
 * @brief  カメラコンポネートクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/24
 *
 * カメラ機能を提供するコンポーネントクラス
 */

#include "pch.h"
#include "Camera.h"

namespace CCC::Components
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	Camera::Camera(CCC::Interfaces::IObject* p_Owner, float eyeSpeedRate, float targetSpeedRate) noexcept :
		UpdataComponentBase(p_Owner),
		m_InitFlag(true),
		m_EyeMoveRate(eyeSpeedRate),
		m_TargetMoveRate(targetSpeedRate)
	{
	}

	Camera::~Camera() noexcept = default;

	void Camera::Update(float elapsedTime)
	{
		// 警告回避
		elapsedTime;

		m_View = DirectX::SimpleMath::Matrix::CreateLookAt(m_EyePosition, m_TargetPosition, DirectX::SimpleMath::Vector3::UnitY);
	}




	// ---------------------------------------------------------------------- //
	// 設定関数
	// ---------------------------------------------------------------------- //

	void Camera::SetCameraPositions(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target)
	{
		if (m_InitFlag)
		{
			m_InitFlag = false;
			m_EyePosition = eye;
			m_TargetPosition = target;
		}

		// カメラの位置を移動
		m_EyePosition += (eye - m_EyePosition) * m_EyeMoveRate;

		// カメラのターゲットの位置を移動
		m_TargetPosition += (target - m_TargetPosition) * m_TargetMoveRate;
	}
}

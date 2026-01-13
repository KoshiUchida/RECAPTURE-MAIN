/**
 * @file   PawnBase.cpp
 *
 * @brief  ポーン基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/24
 */

#include "pch.h"
#include "PawnBase.h"

#include <CCC/Components/Transform.h>

#include <CCC/Managers/CameraManager.h>

/// <summary>
/// 近似値で atan2 を計算する関数
/// </summary>
float FastAtan2(float y, float x)
{
	const float ONEQTR_PI = 0.78539816339f;
	const float THRQTR_PI = 2.35619449019f;

	float abs_y = fabsf(y) + 1e-10f;
	float angle;

	if (x < 0.0f) {
		float r = (x + abs_y) / (abs_y - x);
		angle = THRQTR_PI;
		angle += (0.1963f * r * r - 0.9817f) * r;
	}
	else {
		float r = (x - abs_y) / (x + abs_y);
		angle = ONEQTR_PI;
		angle += (0.1963f * r * r - 0.9817f) * r;
	}

	return y < 0.0f ? -angle : angle;
}


namespace CCC::Bases
{
	PawnBase::PawnBase() :
		ObjectBase(),
		mp_Transform(),
		m_IsSkillActive(false)
	{
		// トランスフォームコンポネートの生成
		mp_Transform = AddComponent<CCC::Components::Transform>("Transform", this);

		// モデルステートの生成
		m_AnimationState = std::make_unique<CCC::Animations::AnimationStateBace>("Paladin", "Paladin_Idle");
	}

	PawnBase::~PawnBase() = default;

	void PawnBase::Update(float elapsedTime)
	{
		// 派生クラスの更新処理を呼び出す
		Process(elapsedTime);

		// ベロシティがゼロでなければ移動処理を行う
		if (m_Velocity != DirectX::SimpleMath::Vector3::Zero)
		{
			DirectX::SimpleMath::Vector3 normalizedDirection = m_Velocity;
			normalizedDirection.Normalize();

			mp_Transform->SetRotateX(FastAtan2(normalizedDirection.z, -normalizedDirection.x));
			mp_Transform->AddRotateX(DirectX::XM_PIDIV2);
			mp_Transform->AddPosition(m_Velocity * elapsedTime);
		}

		// アニメーションステートの更新処理を行う
		m_AnimationState->Update(elapsedTime);
	}

	void PawnBase::Render()
	{
		// カメラ管理クラスの取得
		CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();

		// アニメーションステートの描画処理を行う
		m_AnimationState->Render(mp_Transform->GetWorldMatrix(), p_cm->GetView());
	}

	void PawnBase::Finalize()
	{
	}



	void PawnBase::ResetAnimationSpeed()
	{
		m_AnimationState->ResetAnimationSpeed();
	}

	std::string PawnBase::GetCurrentAnimationName() const
	{
		return m_AnimationState->GetCurrentAnimationName();
	}

	bool PawnBase::EndCurrentAnimation() const
	{
		return m_AnimationState->EndCurrentAnimation();
	}



	void PawnBase::SetAnimationTime(float time)
	{
		m_AnimationState->SetAnimationTime(time);
	}

	void PawnBase::SetPosition(const DirectX::SimpleMath::Vector3& position)
	{
		mp_Transform->SetPosition(position);
	}

	void PawnBase::SetRotate(const DirectX::SimpleMath::Vector3& rotate)
	{
		mp_Transform->SetRotate(rotate);
	}

	void PawnBase::SetScale(float scale)
	{
		mp_Transform->SetScale(scale);
	}

	void PawnBase::SetVelocity(const DirectX::SimpleMath::Vector3& velocity)
	{
		m_Velocity = velocity;
	}

	void PawnBase::SetAnimationSpeed(float speed)
	{
		m_AnimationState->SetAnimationSpeed(speed);
	}



	float PawnBase::GetCurrentAnimationDuration() const
	{
		return static_cast<float>(m_AnimationState->GetCurrentAnimationDuration());
	}

	CCC::Components::Transform* PawnBase::GetTransform() const
	{
		return mp_Transform;
	}

	DirectX::SimpleMath::Vector3 PawnBase::GetPosition() const
	{
		return mp_Transform->GetPosition();
	}

	DirectX::SimpleMath::Vector3 PawnBase::GetVelocity() const
	{
		return m_Velocity;
	}



	void PawnBase::AddPosition(const DirectX::SimpleMath::Vector3& position)
	{
		mp_Transform->AddPosition(position);
	}

	void PawnBase::AddAnimationPlayer(const std::string& animationName)
	{
		m_AnimationState->AddAnimationPlayer(animationName);
	}

	void PawnBase::AddVelocity(const DirectX::SimpleMath::Vector3& velocity)
	{
		m_Velocity += velocity;
	}
}

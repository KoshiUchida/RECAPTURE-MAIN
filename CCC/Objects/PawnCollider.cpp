#include "pch.h"
#include "PawnCollider.h"

#include "PawnManager.h"

namespace CCC::Components
{
	PawnCollider::PawnCollider(CCC::Bases::PawnBase* p_Owner, float colliderRadius, PawnManager* p_Manager) :
		UpdataComponentBase(p_Owner),
		m_Active(true),
		m_Radius(colliderRadius),
		m_TeamID(),
		m_CoolTime(0.0f)
	{
		m_Position     = DirectX::SimpleMath::Vector2(p_Owner->GetPosition().x, p_Owner->GetPosition().z);
		m_TeamID       = p_Owner->GetTeamID();
		m_Velocity     = DirectX::SimpleMath::Vector3::Zero;
		m_OutsideForce = DirectX::SimpleMath::Vector3::Zero;

		// 管理クラスに登録する
		p_Manager->AddPawnCollider(this);
	}

	PawnCollider::~PawnCollider()
	{
	}

	void PawnCollider::Update(float elapsedTime)
	{
		// 時間の経過
		if (m_CoolTime > 0.0f)
		{
			m_CoolTime -= elapsedTime;
			if (m_CoolTime < 0.0f)
				m_CoolTime = 0.0f;
		}


		// オーナーの取得
		CCC::Bases::PawnBase* p_Owner = static_cast<CCC::Bases::PawnBase*>(this->GetOwner());

		// オーナーの座標を保持
		m_Position = DirectX::SimpleMath::Vector2(p_Owner->GetPosition().x, p_Owner->GetPosition().z);

		// オーナーのベロシティを保持
		m_Velocity = p_Owner->GetVelocity();
	}

	DirectX::SimpleMath::Vector3 PawnCollider::GetOutsideForce()
	{
		DirectX::SimpleMath::Vector3 force = m_OutsideForce;

		m_OutsideForce = DirectX::SimpleMath::Vector3::Zero;

		m_CoolTime = 1.5f;

		return force;
	}
}

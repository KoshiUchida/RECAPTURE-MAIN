#include "pch.h"
#include "PawnCollider.h"

#include "PawnManager.h"

#include <CCC/Objects/Characters/Paladin/PaladinCharacter.h>

// 関連のあるコンポネートクラス	
#include <CCC/Components/Transform.h>

namespace CCC::Components
{
	PawnCollider::PawnCollider(CCC::Interfaces::IObject* p_Owner, float colliderRadius, PawnManager* p_Manager) :
		UpdataComponentBase(p_Owner),
		m_Active(true),
		m_Radius(colliderRadius),
		m_TeamID(),
		m_CoolTime(0.0f)
	{
		PaladinCharacter* owner = static_cast<PaladinCharacter*>(p_Owner);

		m_Position     = DirectX::SimpleMath::Vector2(owner->GetTransform()->GetPosition().x, owner->GetTransform()->GetPosition().z);
		m_TeamID       = owner->GetTeamID();
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
		PaladinCharacter* owner = static_cast<PaladinCharacter*>(this->GetOwner());

		// オーナーの座標を保持
		m_Position = DirectX::SimpleMath::Vector2(owner->GetTransform()->GetPosition().x, owner->GetTransform()->GetPosition().z);

		// オーナーのベロシティを保持
		m_Velocity = owner->GetVelocity();
	}

	DirectX::SimpleMath::Vector3 PawnCollider::GetOutsideForce()
	{
		DirectX::SimpleMath::Vector3 force = m_OutsideForce;

		m_OutsideForce = DirectX::SimpleMath::Vector3::Zero;

		m_CoolTime = 1.5f;

		return force;
	}
}

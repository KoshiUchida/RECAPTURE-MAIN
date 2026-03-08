/**
 * @file   ColliderManager.cpp
 *
 * @brief  コライダー管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/02/06
 *
 * コライダーの衝突処理を管理するクラス
 *
 * 2026/02/05
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "ColliderManager.h"

// コライダーコンポーネント
#include <CCC/Components/Colliders/BoxCollider.h>

// コンポーネント
#include <CCC/Components/Transform.h>

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	ColliderManager::ColliderManager() noexcept
	{
	}

	ColliderManager::~ColliderManager() noexcept = default;
	
	void ColliderManager::Update()
	{
		for (int i = 0; i < m_Colliders.size(); i++)
		{
			// コライダー基底コンポネートクラスを取得
			CCC::Bases::ColliderComponentBase* p_ColliderI = static_cast<CCC::Bases::ColliderComponentBase*>(m_Colliders[i]);

			// もし、静的なコライダーだったらコンテニュー
			if (p_ColliderI->IsStatic()) continue;

			for (int j = 0; j < m_Colliders.size(); j++)
			{
				if (i == j) continue;

				// コライダー基底コンポネートクラスを取得
				CCC::Bases::ColliderComponentBase* p_ColliderJ = static_cast<CCC::Bases::ColliderComponentBase*>(m_Colliders[j]);

				// コライダーの衝突処理
				this->Collision(p_ColliderI, p_ColliderJ);
			}
		}
	}

	void ColliderManager::AddCollider(CCC::Interfaces::IComponent* p_ColliderComponent)
	{
		m_Colliders.push_back(p_ColliderComponent);
	}

	void ColliderManager::Clear()
	{
		m_Colliders.clear();
	}



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //

	void ColliderManager::Collision(CCC::Bases::ColliderComponentBase* p_ColliderA, CCC::Bases::ColliderComponentBase* p_ColliderB)
	{
		switch (p_ColliderA->GetType())
		{
		case CCC::Colliders::ColliderType::Box:
			switch (p_ColliderB->GetType())
			{
			case CCC::Colliders::ColliderType::Box:
				BoxToBox(p_ColliderA, p_ColliderB);
				break;
			case CCC::Colliders::ColliderType::Circle:
				// TODO:BoxToCircle
				break;
			default:
				// ありえない処理
				break;
			}

			break;
		case CCC::Colliders::ColliderType::Circle:
			switch (p_ColliderB->GetType())
			{
			case CCC::Colliders::ColliderType::Box:
				// TODO:BoxToCircle
				break;
			case CCC::Colliders::ColliderType::Circle:
				// TODO:CircleToCircle
				break;
			default:
				// ありえない処理
				break;
			}
			break;
		default:
			// ありえない処理
			break;
		}
		return;
	}

	void ColliderManager::BoxToBox(CCC::Bases::ColliderComponentBase* p_ColliderA, CCC::Bases::ColliderComponentBase* p_ColliderB)
	{
		using DirectX::SimpleMath::Vector2;

		// 引き値を判定のためにより具体的にする
		CCC::Components::BoxCollider* p_BoxColliderA = static_cast<CCC::Components::BoxCollider*>(p_ColliderA);
		CCC::Components::BoxCollider* p_BoxColliderB = static_cast<CCC::Components::BoxCollider*>(p_ColliderB);

		// 座標の取得
		Vector2 positionA{ p_BoxColliderA->GetTransform()->GetPositionX(), p_BoxColliderA->GetTransform()->GetPositionZ() };
		Vector2 positionB{ p_BoxColliderB->GetTransform()->GetPositionX(), p_BoxColliderB->GetTransform()->GetPositionZ() };
		Vector2 diffPosition{ positionB - positionA };

		// 重なる範囲を計算
		Vector2 sizeA{ p_BoxColliderA->GetSize() * 0.5f };
		Vector2 sizeB{ p_BoxColliderB->GetSize() * 0.5f };

		// AABBとAABBの重なり判定
		float overlapX = (sizeA.x + sizeB.x) - std::fabsf(diffPosition.x);
		float overlapY = (sizeA.y + sizeB.y) - std::fabsf(diffPosition.y);
		if (overlapX <= 0.0f || overlapY <= 0.0f)
			return;

		// 押し出しベクトルの計算
		DirectX::SimpleMath::Vector3 mtv = DirectX::SimpleMath::Vector3::Zero;
		if (overlapX < overlapY)
			mtv.x = copysignf(1.0f, diffPosition.x) * overlapX;
		else
			mtv.z = copysignf(1.0f, diffPosition.y) * overlapY;

		// 動的同士の押し出し
		if ((!p_BoxColliderA->IsStatic()) && (!p_BoxColliderB->IsStatic()))
		{
			p_BoxColliderA->GetTransform()->AddPosition(-mtv * 0.5f);
			p_BoxColliderB->GetTransform()->AddPosition( mtv * 0.5f);
			return;
		}

		// 片方が動的な押し出し
		if (!p_BoxColliderA->IsStatic())
		{
			p_BoxColliderA->GetTransform()->AddPosition(-mtv);
			return;
		}

		if (!p_BoxColliderB->IsStatic())
		{
			p_BoxColliderB->GetTransform()->AddPosition(mtv);
			return;
		}
	}
}

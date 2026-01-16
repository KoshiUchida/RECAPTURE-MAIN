#include "pch.h"
#include "PawnManager.h"

PawnManager::PawnManager()
{
}

PawnManager::~PawnManager() = default;

void PawnManager::Initialize()
{
}

void PawnManager::Update(float elapsedTime)
{
	// 警告回避
	elapsedTime;

	for (int i = 0; i < m_PawnColliders.size(); i++)
	{
		// インデックスiのコライダー
		CCC::Components::PawnCollider* p_ICollider = m_PawnColliders[i];

		// すでに外力が入力されていたらコンティニュー
		if (p_ICollider->IsSetOutsideForce()) continue;

		for (int j = 0; j < m_PawnColliders.size(); j++)
		{
			// 同じインデックスだったらコンティニュー
			if (i == j) continue;


			// インデックスjのコライダー
			CCC::Components::PawnCollider* p_JCollider = m_PawnColliders[j];

			// すでに外力が入力されていたらコンティニュー
			if (p_JCollider->IsSetOutsideForce()) continue;

			// 同じチームIDだったらコンティニュー
			if (p_ICollider->GetTeamID() == p_JCollider->GetTeamID()) continue;


			// ---------------------------------------------------------------------- //
			// 円の重なり判定
			// ---------------------------------------------------------------------- //

			// 半径の収集
			float radius = p_JCollider->GetRadius() + p_ICollider->GetRadius();

			// ２点間の差分
			DirectX::SimpleMath::Vector2 positionDiff = p_JCollider->GetPosition() - p_ICollider->GetPosition();
			float diff = positionDiff.Length();

			// もし、差分が半径の集まりに比べて小さかったら、重なっている
			if (diff <= radius)
			{
				// 衝突法線を作る
				positionDiff.Normalize();

				// 相対速度を作る
				DirectX::SimpleMath::Vector2 velocityDiff =
					DirectX::SimpleMath::Vector2(p_JCollider->GetVelocity().x, p_JCollider->GetVelocity().z) -
					DirectX::SimpleMath::Vector2(p_ICollider->GetVelocity().x, p_ICollider->GetVelocity().z);
				velocityDiff.Normalize();

				// 衝突してくるかを内積から求める
				if (velocityDiff.Dot(positionDiff) < 0.0f)
				{
					p_ICollider->SetOutsideForce(p_JCollider->GetVelocity());
				}
			}
		}
	}
}

void PawnManager::Render()
{
}

void PawnManager::Finalize()
{
	m_PawnColliders.clear();
}

void PawnManager::AddPawnCollider(CCC::Components::PawnCollider* p_PawnCollider)
{
	m_PawnColliders.push_back(p_PawnCollider);
}

/**
 * @file   AABBBoxCollider.cpp
 *
 * @brief  AABBのボックスコライダーコンポネートクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/02/08
 *
 * AABBのボックスコライダーコンポネートクラス
 *
 * 2026/02/06
 * 作成
 * 
 * 2026/02/08
 * 取得関数の実装
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "BoxCollider.h"

namespace CCC::Components
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	BoxCollider::BoxCollider(
		CCC::Interfaces::IObject*   p_Owner,
		CCC::Components::Transform* p_Transform,
		bool isStatic,
		float width,
		float height) :
		ColliderComponentBase(p_Owner, p_Transform, isStatic, CCC::Colliders::ColliderType::Box),
		m_Width (width),
		m_Height(height)
	{
	}

	CCC::Components::BoxCollider::~BoxCollider() noexcept = default;



	// ---------------------------------------------------------------------- //
	// 取得関数
	// ---------------------------------------------------------------------- //
	
	float BoxCollider::GetWidth() const
	{
		return m_Width;
	}
	float BoxCollider::GetHeight() const
	{
		return m_Height;
	}
	DirectX::SimpleMath::Vector2 BoxCollider::GetSize() const
	{
		return DirectX::SimpleMath::Vector2(m_Width, m_Height);
	}
}

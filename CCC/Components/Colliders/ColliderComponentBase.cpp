/**
 * @file   ColliderComponentBase.cpp
 *
 * @brief  コライダー基底コンポネートクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/02/08
 *
 * コライダーの基底クラスとなるコンポネートクラス
 *
 * 2026/02/05
 * 作成
 * 
 * 2026/02/08
 * 自身を管理クラスに登録する機能をコンストラクタに実装
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "ColliderComponentBase.h"

// コンポネート
#include <CCC/Components/Transform.h>

// 管理クラス
#include <CCC/Managers/ManagersHub.h>
#include <CCC/Interfaces/IColliderManager.h>

namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	ColliderComponentBase::ColliderComponentBase(
		CCC::Interfaces::IObject*           p_Owner,
		CCC::Components::Transform*         p_Transform,
		bool                                isStatic,
		const CCC::Colliders::ColliderType& type
	) :
		ComponentBase(p_Owner),
		mp_Transform (p_Transform),
		m_IsStatic   (isStatic),
		m_Type       (type)
	{
		// 管理クラスに追加
		CCC::Managers::ManagersHub* p_mh = CCC::Managers::ManagersHub::Get();
		p_mh->GetColliderManager()->AddCollider(this);
	}

	ColliderComponentBase::~ColliderComponentBase() noexcept = default;

	CCC::Components::Transform* ColliderComponentBase::GetTransform()
	{
		return mp_Transform;
	}

	bool ColliderComponentBase::IsStatic() const
	{
		return m_IsStatic;
	}

	CCC::Colliders::ColliderType ColliderComponentBase::GetType() const
	{
		return m_Type;
	}
}

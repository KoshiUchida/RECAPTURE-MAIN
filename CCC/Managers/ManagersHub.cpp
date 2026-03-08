/**
 * @file   ManagersHub.cpp
 *
 * @brief  管理中継クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/02/08
 *
 * 管理クラスを中継するためのクラス
 *
 * 2026/02/08
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "ManagersHub.h"

// 管理クラス
#include <CCC/Interfaces/IColliderManager.h>

namespace CCC::Managers
{
	// 実体の初期化
	std::unique_ptr<ManagersHub> ManagersHub::s_Instance = nullptr;

	ManagersHub::ManagersHub(
		CCC::Interfaces::IColliderManager* p_ColliderManager
	) :
		mp_ColliderManager(p_ColliderManager)
	{
	}

	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	ManagersHub::~ManagersHub() = default;

	void ManagersHub::Generate(CCC::Interfaces::IColliderManager* p_ColliderManager)
	{
		if (s_Instance)
			DX::ThrowIfFailed(E_ACCESSDENIED);

		s_Instance.reset(new ManagersHub(p_ColliderManager));
	}

	ManagersHub* ManagersHub::Get()
	{
		if (!s_Instance)
		{
			DX::ThrowIfFailed(E_ACCESSDENIED);

			return nullptr;
		}

		return s_Instance.get();
	}

	CCC::Interfaces::IColliderManager* ManagersHub::GetColliderManager() const
	{
		return mp_ColliderManager;
	}
}

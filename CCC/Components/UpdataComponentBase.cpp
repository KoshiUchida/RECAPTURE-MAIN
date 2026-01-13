/**
 * @file   UpadataComponentBase.cpp
 *
 * @brief  更新処理を持つコンポーネント基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/11
 *
 * 更新処理の必要があるコンポーネントの基底クラス
 */

#include "pch.h"
#include "UpdataComponentBase.h"

// 管理クラス
#include <CCC/Managers/ComponentManager.h>

namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	UpdataComponentBase::UpdataComponentBase(CCC::Interfaces::IObject* p_Owner) noexcept :
		ComponentBase(p_Owner)
	{
		// コンポーネント管理クラスを取得する
		CCC::Managers::ComponentManager* p_ComponentManager = CCC::Managers::ComponentManager::GetInstance();

		// コンポーネント管理クラスに自身を登録する
		p_ComponentManager->AddUpdataComponent(this);
	}

	UpdataComponentBase::~UpdataComponentBase() noexcept
	{
		// コンポーネント管理クラスを取得する
		CCC::Managers::ComponentManager* p_ComponentManager = CCC::Managers::ComponentManager::GetInstance();

		// コンポーネント管理クラスの自身を解除させる
		p_ComponentManager->SubUpdataComponent(this);
	}
}

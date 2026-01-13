/**
 * @file   ComponentBase.cpp
 *
 * @brief  コンポーネント基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/11
 *
 * コンポネートの基底クラス
 *
 * すべてのコンポネートはこのクラスから継承されていなければならない
 */

#include "pch.h"
#include "ComponentBase.h"

namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	ComponentBase::ComponentBase(CCC::Interfaces::IObject* p_Owner) noexcept :
		mp_Owner{ p_Owner }
	{
	}

	ComponentBase::~ComponentBase() noexcept = default;
}

/**
 * @file   ResourceBase.cpp
 *
 * @brief  リソース基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/04
 * リソースを保持する基底クラス
 * m_UseScene扱うシーン名を保持できるようになっている
 */

#include "pch.h"
#include "ResourceBase.h"

namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	ResourceBase::ResourceBase(const wchar_t* path, const std::string& useScene) :
		m_Path(path),
		m_UseScene(useScene)
	{
	}

	ResourceBase::~ResourceBase() = default;
}

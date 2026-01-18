/**
 * @file   ObjectBase.cpp
 *
 * @brief  オブジェクト基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/14
 */

#include "pch.h"
#include "ObjectBase.h"

namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	ObjectBase::ObjectBase()
	{
	}

	ObjectBase::~ObjectBase()
	{
		m_Components.clear();
	}
}

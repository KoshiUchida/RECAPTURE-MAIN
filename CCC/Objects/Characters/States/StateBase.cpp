/**
 * @file   StateBase.cpp
 *
 * @brief  キャラクターのステート基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/02/27
 * 作成
 * 
 * 2026/03/08
 * 共通化により、名称の「Paladin」を削除、
 * キャラクタークラスの共通化により、PaladinCharacteクラスからCharacterBaseクラスに変更
 */

// プリコンパイル済みヘッダーファイル
#include "pch.h"

// 基底クラス
#include "StateBase.h"

// 前方宣言
#include "../CharacterBase.h"


namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	StateBase::StateBase(CharacterBase* p_Owner) :
		mp_Owner(p_Owner)
	{
	}

	StateBase::~StateBase() = default;
	
	CharacterBase* StateBase::GetOwner()
	{
		return mp_Owner;
	}
}

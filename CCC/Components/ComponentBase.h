/**
 * @file   ComponentBase.h
 *
 * @brief  コンポーネント基底クラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2026/01/14
 *
 * コンポネートの基底クラス
 * 
 * すべてのコンポネートはこのクラスから継承されていなければならない
 * 
 * 2025/12/11
 * 作成
 * 
 * 2026/01/14
 * オーナーの取得を定数ポインタからポインタを取得するように修正
 */

#pragma once
#ifndef CONPONENT_BASE_DEFINE
#define CONPONENT_BASE_DEFINE

// 基底クラス
#include <CCC/Interfaces/IComponent.h>

namespace CCC
{
	namespace Bases
	{
		/// <summary>
		/// コンポネート基底クラス
		/// </summary>
		class ComponentBase :
			public CCC::Interfaces::IComponent
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			ComponentBase(CCC::Interfaces::IObject* p_Owner) noexcept;
			virtual ~ComponentBase() noexcept;
			CCC::Interfaces::IObject* GetOwner() override {
				return mp_Owner;
			}



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			CCC::Interfaces::IObject* mp_Owner;
		};
	}
}

#endif // !CONPONENT_BASE_DEFINE

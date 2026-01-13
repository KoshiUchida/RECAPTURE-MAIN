/**
 * @file   UpadataComponentBase.h
 *
 * @brief  更新処理を持つコンポーネント基底クラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2025/12/11
 *
 * 更新処理付きコンポーネントの基底クラス
 */

#pragma once
#ifndef UPDATA_COMPONENT_BASE
#define UPDATA_COMPONENT_BASE

// 基底クラス
#include <CCC/Components/ComponentBase.h>
#include <CCC/Interfaces/IUpdataComponent.h>

namespace CCC
{
	namespace Bases
	{
		/// <summary>
		/// 更新処理を持つコンポーネント基底クラス
		/// </summary>
		class UpdataComponentBase :
			public CCC::Bases::ComponentBase,
			public CCC::Interfaces::IUpdataComponent
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="p_Owner">このコンポーネントを持つオブジェクトへのポインタ</param>
			UpdataComponentBase(CCC::Interfaces::IObject* p_Owner) noexcept;

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~UpdataComponentBase() noexcept;
		};
	}
}

#endif // ! UPDATA_COMPONENT_BASE

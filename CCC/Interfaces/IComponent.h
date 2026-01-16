/**
 * @file   IComponent.h
 *
 * @brief  コンポーネントインターフェースのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2025/12/11
 *
 * コンポネートの抽象クラス
 * すべてのコンポネートクラスはこのインターフェースから実装がなされていなければならない
 * 
 * オーナ－の取得関数を必ず実装しなければならない
 * 
 * 2025/12/11
 * 作成
 * 
 * 2026/01/14
 * オーナーの取得を定数ポインタからポインタを取得するように修正
 */

#pragma once

#include <CCC/Interfaces/IObject.h>

namespace CCC
{
	namespace Interfaces
	{
		/// <summary>
		/// コンポネートインターフェースクラス
		/// </summary>
		class IComponent
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~IComponent() noexcept = default;

			/// <summary>
			/// このコンポネートを保持するオブジェクトを取得する
			/// </summary>
			virtual CCC::Interfaces::IObject* GetOwner() = 0;
		};
	}
}

/**
 * @file   IUpdataComponent.h
 *
 * @brief  更新処理付きコンポーネントインターフェースのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2025/12/11
 *
 *  更新処理付きコンポネートの抽象クラス
 * すべての更新処理付きコンポネートクラスはこのインターフェースを実装していなければならない
 *
 * 更新処理関数を必ず実装しなければならない
 */

#pragma once

namespace CCC
{
	namespace Interfaces
	{
		/// <summary>
		/// 更新処理付きコンポネートインターフェースクラス
		/// </summary>
		class IUpdataComponent
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~IUpdataComponent() noexcept = default;

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			virtual void Update(float elapsedTime) = 0;
		};
	}
}

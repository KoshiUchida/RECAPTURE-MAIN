/**
 * @file   IObject.h
 *
 * @brief  オブジェクトインターフェースのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/11/26
 * オブジェクトのインターフェース
 */

#pragma once

namespace CCC
{
	namespace Interfaces
	{
		/// <summary>
		/// オブジェクトインターフェースクラス
		/// </summary>
		class IObject
		{
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~IObject() = default;

			/// <summary>
			/// 初期化処理
			/// </summary>
			virtual void Initialize() = 0;
			
			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			virtual void Update(float elapsedTime) = 0;
			
			/// <summary>
			/// 描画処理
			/// </summary>
			virtual void Render() = 0;
			
			/// <summary>
			/// 終了処理
			/// </summary>
			virtual void Finalize() = 0;
		};
	}
}

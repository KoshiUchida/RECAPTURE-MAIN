/**
 * @file   IScene.h
 *
 * @brief  シーンインターフェースのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/11/14
 * シーンを一元管理するための抽象クラス
 * シーンの基底クラス作成する場合はこのクラスを継承しなければならない
 * Initialize、Update、Redner、Finalize
 * 以上の関数は必ず実装（override）しなければならない
 */

#pragma once

namespace CCC
{
	namespace Interfaces
	{
		class IScene
		{
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~IScene() = default;

			/// <summary>
			/// 初期化処理
			/// </summary>
			virtual void Initialize() = 0;

			/// <summary>
			/// 更新処理
			/// </summary>
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

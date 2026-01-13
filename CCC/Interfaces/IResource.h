/**
 * @file   IResource.h
 *
 * @brief  リソースインターフェースのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/12/10
 * リソースは必ずこのインターフェースから継承していないといけない。
 */
#pragma once

namespace CCC
{
	namespace Interfaces
	{
		class IResource
		{
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~IResource() = default;

			/// <summary>
			/// リソースの読み込み
			/// </summary>
			virtual void Load() = 0;

			/// <summary>
			/// リソースの破棄
			/// </summary>
			virtual void Unload() = 0;

			/// <summary>
			/// 使用するシーン名の取得
			/// </summary>
			virtual const std::string& GetUseScene() const = 0;
		};
	}
}

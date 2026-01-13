/**
 * @file   ResourceBase.h
 *
 * @brief  リソース基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/10
 * リソースを保持する基底クラス
 * m_UseScene扱うシーン名を保持できるようになっている
 */

#pragma once
#ifndef RESOURCE_BACE
#define RESOURCE_BACE

 // リソースインターフェース
#include <CCC/Interfaces/IResource.h>

namespace CCC
{
	namespace Bases
	{
		/// <summary>
		/// リソースの基底クラス
		/// </summary>
		class ResourceBase :
			public Interfaces::IResource
		{
		public:
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ResourceBase(const wchar_t* path, const std::string& useScene = "");

			/// <summary>
			///  デストラクタ
			/// </summary>
			virtual ~ResourceBase();

			/// <summary>
			/// リソースの読み込み
			/// </summary>
			virtual void Load() override = 0;

			/// <summary>
			/// リソースの破棄
			/// </summary>
			void Unload() override = 0;

			/// <summary>
			/// リソースへのパスの取得
			/// </summary>
			/// <returns></returns>
			const std::wstring& GetPath() const { return m_Path; }

			/// <summary>
			/// 使用するシーンの名前の取得
			/// </summary>
			/// <returns>使用するシーンの名前</returns>
			const std::string& GetUseScene() const override final { return m_UseScene; }



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// このリソースへのパス
			std::wstring m_Path;

			// このリソースを扱うシーン
			std::string m_UseScene;
		};
	}
}

#endif // !RESOURCE_BACE

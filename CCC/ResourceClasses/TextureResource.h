/**
 * @file   TextureResource.h
 *
 * @brief  テクスチャリソースのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/12
 * テクスチャを保持するクラス
 * m_UseScene扱うシーン名を保持できるようになっている
 * 
 * 2025/11/25
 * 作成
 * 
 * 2026/01/12
 * 画像サイズを保持取得する機能を実装
 */

#pragma once
#ifndef TEXTURE_RESOURCE_DEFINED
#define TEXTURE_RESOURCE_DEFINED

// リソースの基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

namespace CCC
{
	namespace Resources
	{
		/// <summary>
		/// テクスチャのリソースクラス
		/// </summary>
		class TextureResource final :
			public Bases::ResourceBase
		{
		public:
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// コンストラクタ
			/// </summary>
			TextureResource(const wchar_t* path, const std::string& useScene = "");

			/// <summary>
			///  デストラクタ
			/// </summary>
			virtual ~TextureResource();

			/// <summary>
			/// リソースの読み込み
			/// </summary>
			void Load() override;

			/// <summary>
			/// リソースの破棄
			/// </summary>
			void Unload() override;

			/// <summary>
			/// アドレスの取得
			/// </summary>
			ID3D11ShaderResourceView** GetAddressOf()
			{
				return m_Texture.GetAddressOf();
			}

			/// <summary>
			/// テクスチャへのポインタの取得
			/// </summary>
			ID3D11ShaderResourceView* Get()
			{
				return m_Texture.Get();
			}

			/// <summary>
			/// 画像サイズの取得
			/// </summary>
			DirectX::XMUINT2 GetSize() const
			{
				return m_Size;
			}

			/// <summary>
			/// 画像横幅サイズの取得
			/// </summary>
			UINT GetWidth() const
			{
				return GetSize().x;
			}

			/// <summary>
			/// 画像縦幅サイズの取得
			/// </summary>
			UINT GetHeight() const
			{
				return GetSize().y;
			}



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// テクスチャ（リソース）
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

			// 画像サイズ
			DirectX::XMUINT2 m_Size;
		};
	}
}

#endif // !TEXTURE_RESOURCE_DEFINED

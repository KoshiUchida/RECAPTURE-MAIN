/**
 * @file   TextureResource.cpp
 *
 * @brief  テクスチャリソースのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/13
 * テクスチャを保持するクラス
 * 
 * 2025/11/25
 * 作成
 * 
 * 2025/12/04
 * Load関数の
 * 画像の読み込み機能を修正
 * 
 * 2026/01/13
 * Load関数の
 * 画像読み込み後に画像サイズを取得するよう修正
 */

#include "pch.h"
#include "TextureResource.h"

#include <WICTextureLoader.h>

// リソースマネージャ
#include <CCC/Managers/ResourceManager.h>

namespace CCC::Resources
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	TextureResource::TextureResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene),
		m_Size(0, 0)
	{
	}

	TextureResource::~TextureResource()
	{
		Unload();
	}

	void TextureResource::Load()
	{
		CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();

		DX::ThrowIfFailed(
			DirectX::CreateWICTextureFromFile(
				p_rm->GetD3DDevice(),
				GetPath().c_str(),
				nullptr,
				m_Texture.ReleaseAndGetAddressOf()
			)
		);

		Microsoft::WRL::ComPtr<ID3D11Resource> res;
		m_Texture->GetResource(res.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2d;
		DX::ThrowIfFailed(
			res.As(&tex2d)
		);

		D3D11_TEXTURE2D_DESC desc = D3D11_TEXTURE2D_DESC{};
		tex2d->GetDesc(&desc);

		m_Size = DirectX::XMUINT2(desc.Width, desc.Height);
	}

	void TextureResource::Unload()
	{
		m_Texture.Reset();
	}
}

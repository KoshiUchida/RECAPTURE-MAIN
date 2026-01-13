/**
 * @file   PixelShaderResource.cpp
 *
 * @brief  ピクセルシェーダリソースのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/12
 * ピクセルシェーダを保持するクラス
 * 
 * 2026/01/09
 * 作成
 * 
 * 2026/01/10
 * データ破棄のための関数が間違っていたため修正
 * 
 * 2026/01/12
 * Load関数をシェーダとして読み込みからバイトコードとして読み込むように修正
 */

#include "pch.h"
#include "PixelShaderResource.h"

#include <d3dcompiler.h>

// リソースマネージャ
#include <CCC/Managers/ResourceManager.h>

namespace CCC::Resources
{
	PixelShaderResource::PixelShaderResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene)
	{
		m_Shader.Reset();
		m_Bytecode.Reset();
	}

	PixelShaderResource::~PixelShaderResource()
	{
		this->Unload();
	}

	void PixelShaderResource::Load()
	{
		// デバイスの取得
		ID3D11Device1* device = CCC::Managers::ResourceManager::GetInstance()->GetD3DDevice();

		// バイトコード
		Microsoft::WRL::ComPtr<ID3DBlob> blob;

		// リソースからデータを読み込む
		DX::ThrowIfFailed(
			D3DReadFileToBlob(this->GetPath().c_str(), blob.GetAddressOf())
		);

		// PSデータの作成
		DX::ThrowIfFailed(
			device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_Shader.ReleaseAndGetAddressOf())
		);

		// バイトコードを保持
		m_Bytecode = blob;
	}

	void PixelShaderResource::Unload()
	{
		m_Shader.Reset();
		m_Bytecode.Reset();
	}

	ID3D11PixelShader** PixelShaderResource::GetAddressOf()
	{
		return m_Shader.GetAddressOf();
	}

	ID3D11PixelShader* PixelShaderResource::Get()
	{
		return m_Shader.Get();
	}
}

/**
 * @file   VertexShaderResource.cpp
 *
 * @brief  頂点シェーダリソースのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/10
 * 頂点シェーダを保持するクラス
 * 
 * 2026/01/09
 * 作成
 * 
 * 2026/01/10
 * データ破棄の関数が間違っていたため、修正
 */

#include "pch.h"
#include "VertexShaderResource.h"

#include <d3dcompiler.h>
#include <d3d11_1.h>

// リソースマネージャ
#include <CCC/Managers/ResourceManager.h>

namespace CCC::Resources
{
	VertexShaderResource::VertexShaderResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene)
	{
	}

	VertexShaderResource::~VertexShaderResource()
	{
		this->Unload();
	}

	void VertexShaderResource::Load()
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
			device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_Shader.ReleaseAndGetAddressOf())
		);

		// バイトコードを保持
		m_Bytecode = blob;
	}
	void VertexShaderResource::Unload()
	{
		m_Shader.Reset();
	}

	ID3D11VertexShader** VertexShaderResource::GetAddressOf()
	{
		return m_Shader.GetAddressOf();
	}

	ID3D11VertexShader* VertexShaderResource::Get()
	{
		return m_Shader.Get();
	}
}

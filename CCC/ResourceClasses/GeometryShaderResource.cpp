/**
 * @file   GeometryShaderResource.cpp
 *
 * @brief  ジオメトリシェーダリソースのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/09
 * ジオメトリシェーダを保持するクラス
 * 
 * 2026/01/09
 * 作成
 * 
 * 2026/01/10
 * データ破棄の関数が間違っていたため修正
 */

#include "pch.h"
#include "GeometryShaderResource.h"

#include <DX/ReadData.h>

// リソースマネージャ
#include <CCC/Managers/ResourceManager.h>

namespace CCC::Resources
{
	GeometryShaderResource::GeometryShaderResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene)
	{
	}

	GeometryShaderResource::~GeometryShaderResource()
	{
		this->Unload();
	}

	void GeometryShaderResource::Load()
	{
		std::vector<uint8_t> shader = DX::ReadData(this->GetPath().c_str());

		ID3D11Device1* device = CCC::Managers::ResourceManager::GetInstance()->GetD3DDevice();

		DX::ThrowIfFailed(
			device->CreateGeometryShader(shader.data(), shader.size(), nullptr, m_Shader.ReleaseAndGetAddressOf())
		);
	}

	void GeometryShaderResource::Unload()
	{
		m_Shader.Reset();
	}

	ID3D11GeometryShader** GeometryShaderResource::GetAddressOf()
	{
		return m_Shader.GetAddressOf();
	}

	ID3D11GeometryShader* GeometryShaderResource::Get()
	{
		return m_Shader.Get();
	}
}

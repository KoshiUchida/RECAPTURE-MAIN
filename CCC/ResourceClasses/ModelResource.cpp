/**
 * @file   ModelResource.cpp
 *
 * @brief  モデルリソースのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/04
 * モデルを保持するクラス
 */

#include "pch.h"
#include "ModelResource.h"

#include <CCC/Managers/ResourceManager.h>
#include <filesystem>

namespace CCC::Resources
{
	ModelResource::ModelResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene)
	{
	}

	ModelResource::~ModelResource()
	{
		Unload();
	}

	void ModelResource::Load()
	{
		// デバイスの取得
		ID3D11Device1* device = CCC::Managers::ResourceManager::GetInstance()->GetD3DDevice();

		// モデルパスの取得
		std::filesystem::path modelPath = GetPath();

		// 親ディレクトリだけ取り出す
		std::filesystem::path dirPath = modelPath.parent_path();
		
		// ファクトリの生成
		std::unique_ptr<DirectX::EffectFactory> fx
			= std::make_unique<DirectX::EffectFactory>(device);
		fx->SetDirectory(dirPath.c_str());

		// モデルの生成
		m_Model = DirectX::Model::CreateFromSDKMESH(
			device, GetPath().c_str(), *fx
		);
	}

	void ModelResource::Unload()
	{
		m_Model.reset();
	}
}

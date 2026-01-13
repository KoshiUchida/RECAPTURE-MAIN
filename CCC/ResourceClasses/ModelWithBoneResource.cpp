/**
 * @file   ModelWithBoneResource.cpp
 *
 * @brief  モデル(ボーン付き)リソースクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/04
 */

#include "pch.h"
#include "ModelWithBoneResource.h"

#include <CCC/Managers/ResourceManager.h>

#include <filesystem>

namespace CCC::Resources
{
	ModelWithBoneResource::ModelWithBoneResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene),
		m_IsBinded(false)
	{
	}

	ModelWithBoneResource::~ModelWithBoneResource()
	{
		Unload();
	}

	void ModelWithBoneResource::Load()
	{
		using namespace DirectX;

		// デバイスの取得
		ID3D11Device1* device = CCC::Managers::ResourceManager::GetInstance()->GetD3DDevice();

		// モデルパスの取得
		std::filesystem::path modelPath = GetPath();

		// 親ディレクトリだけ取り出す
		std::filesystem::path dirPath = modelPath.parent_path();

		// ファクトリの生成
		std::unique_ptr<EffectFactory> fx
			= std::make_unique<EffectFactory>(device);
		fx->SetDirectory(dirPath.c_str());


		// モデルの読み込み
		m_Model = Model::CreateFromSDKMESH(
			device, modelPath.c_str(), *fx.get(),
			ModelLoader_Clockwise | ModelLoader_IncludeBones);

		m_DrawBones = ModelBone::MakeArray(m_Model->bones.size());

		m_Model->UpdateEffects([&](IEffect* effect)
			{
				auto skin = dynamic_cast<SkinnedEffect*>(effect);
				if (skin)
				{
					skin->SetPerPixelLighting(true);
				}
			}
		);
	}

	void ModelWithBoneResource::Unload()
	{
		m_Model.reset();
	}
}

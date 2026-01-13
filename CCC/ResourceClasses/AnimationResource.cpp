/**
 * @file   AnimationResource.cpp
 *
 * @brief  モデルアニメーションリソースクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/10
 */
#include "pch.h"
#include "AnimationResource.h"

#include <fstream>

#include <CCC/ResourceClasses/ModelWithBoneResource.h>

namespace CCC::Resources
{
#pragma pack(push,8)

	constexpr uint32_t SDKMESH_FILE_VERSION = 101;
	constexpr uint32_t MAX_FRAME_NAME = 100;

	struct SDKANIMATION_FILE_HEADER
	{
		uint32_t Version;
		uint8_t  IsBigEndian;
		uint32_t FrameTransformType;
		uint32_t NumFrames;
		uint32_t NumAnimationKeys;
		uint32_t AnimationFPS;
		uint64_t AnimationDataSize;
		uint64_t AnimationDataOffset;
	};

	static_assert(sizeof(SDKANIMATION_FILE_HEADER) == 40, "SDK Mesh structure size incorrect");

	struct SDKANIMATION_DATA
	{
		DirectX::XMFLOAT3 Translation;
		DirectX::XMFLOAT4 Orientation;
		DirectX::XMFLOAT3 Scaling;
	};

	static_assert(sizeof(SDKANIMATION_DATA) == 40, "SDK Mesh structure size incorrect");

	struct SDKANIMATION_FRAME_DATA
	{
		char FrameName[MAX_FRAME_NAME];
		union
		{
			uint64_t DataOffset;
			SDKANIMATION_DATA* pAnimationData;
		};
	};

	static_assert(sizeof(SDKANIMATION_FRAME_DATA) == 112, "SDK Mesh structure size incorrect");

#pragma pack(pop)



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	AnimationResource::AnimationResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene),
		m_DataSize(0),
		m_IsBinded(false)
	{
	}

	AnimationResource::~AnimationResource()
	{
		Unload();
	}

	void AnimationResource::Load()
	{
		Unload();

		PrivLoad();
	}

	void AnimationResource::Unload()
	{
		m_DataSize = 0;
		m_Data.reset();
		m_BoneToTrack.clear();
		m_IsBinded = false;
	}

	bool AnimationResource::Bind(CCC::Interfaces::IResource* p_ModelResource)
	{
		// アニメーションリソースが読み込まれていなければバインドできない
		assert(IsLoaded());

		// バインド済みなら何もしない
		if (m_IsBinded) return true;

		// モデルリソースの型を変換
		CCC::Resources::ModelWithBoneResource* p_Model = dynamic_cast<CCC::Resources::ModelWithBoneResource*>(p_ModelResource);

		auto header = reinterpret_cast<const SDKANIMATION_FILE_HEADER*>(m_Data.get());
		assert(header->Version == SDKMESH_FILE_VERSION);
		auto frameData = reinterpret_cast<SDKANIMATION_FRAME_DATA*>(m_Data.get() + header->AnimationDataOffset);

		m_BoneToTrack.resize(p_Model->GetBoneCount());
		for (auto& it : m_BoneToTrack)
		{
			it = DirectX::ModelBone::c_Invalid;
		}

		bool result = false;

		for (size_t j = 0; j < header->NumFrames; ++j)
		{
			uint64_t offset = sizeof(SDKANIMATION_FILE_HEADER) + frameData[j].DataOffset;
			uint64_t end = offset + sizeof(SDKANIMATION_DATA) * uint64_t(header->NumAnimationKeys);
			if (end > UINT32_MAX
				|| end > m_DataSize)
				throw std::runtime_error("Animation file invalid");

			frameData[j].pAnimationData = reinterpret_cast<SDKANIMATION_DATA*>(m_Data.get() + offset);

			wchar_t frameName[MAX_FRAME_NAME] = {};
			MultiByteToWideChar(CP_UTF8, 0, frameData[j].FrameName, -1, frameName, MAX_FRAME_NAME);

			size_t count = 0;
			for (const auto& it : p_Model->GetBones())
			{
				if (_wcsicmp(frameName, it.name.c_str()) == 0)
				{
					m_BoneToTrack[count] = static_cast<uint32_t>(j);
					result = true;
					break;
				}

				++count;
			}
		}


		//	バインド済みフラグを立てる
		m_IsBinded = true;

		return result;
	}

	HRESULT AnimationResource::PrivLoad()
	{
		if (GetPath().empty())
			return E_INVALIDARG;

		std::ifstream inFile(GetPath().c_str(), std::ios::in | std::ios::binary | std::ios::ate);
		if (!inFile)
			return E_FAIL;

		const std::streampos len = inFile.tellg();
		if (!inFile)
			return E_FAIL;

		if (len > UINT32_MAX)
			return HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);

		if (static_cast<size_t>(len) < sizeof(SDKANIMATION_FILE_HEADER))
			return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);

		std::unique_ptr<uint8_t[]> blob(new (std::nothrow) uint8_t[size_t(len)]);
		if (!blob)
			return E_OUTOFMEMORY;

		inFile.seekg(0, std::ios::beg);
		if (!inFile)
			return E_FAIL;

		inFile.read(reinterpret_cast<char*>(blob.get()), len);
		if (!inFile)
			return E_FAIL;

		inFile.close();

		auto header = reinterpret_cast<const SDKANIMATION_FILE_HEADER*>(blob.get());

		if (header->Version != SDKMESH_FILE_VERSION
			|| header->IsBigEndian != 0
			|| header->FrameTransformType != 0 /*FTT_RELATIVE*/
			|| header->NumAnimationKeys == 0
			|| header->NumFrames == 0
			|| header->AnimationFPS == 0)
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

		uint64_t dataSize = header->AnimationDataOffset + header->AnimationDataSize;
		if (dataSize > uint64_t(len))
			return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);

		m_Data.swap(blob);
		m_DataSize = static_cast<size_t>(len);

		return S_OK;
	}

	double AnimationResource::GetDuration() const
	{
		if (!IsLoaded())
			return 0.0;
		auto header = reinterpret_cast<const SDKANIMATION_FILE_HEADER*>(m_Data.get());
		return double(header->NumAnimationKeys) / double(header->AnimationFPS);
	}
}

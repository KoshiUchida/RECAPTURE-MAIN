/**
 * @file   AnimationPlayer.cpp
 *
 * @brief  アニメーションプレイヤーのソースファイル
 *
 * @author CatCode
 *
 * @date    2025/12/10
 */

#include "pch.h"
#include "AnimationPlayer.h"

#include <cassert>
#include <stdexcept>

// 管理クラス
#include <CCC/Managers/ResourceManager.h>

// 前方宣言
#include <CCC/ResourceClasses/AnimationResource.h>
#include <CCC/ResourceClasses/ModelWithBoneResource.h>

namespace CCC::Animations
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



	AnimationPlayer::AnimationPlayer(const std::string& resourceName) :
		mp_AnimationResource(nullptr),
		m_Time(0.0),
		m_Duration(0.0),
		m_PrevAnimTime(0.0),
		m_IsEnd(false)
	{
		mp_AnimationResource = CCC::Managers::ResourceManager::GetInstance()->GetResource<CCC::Resources::AnimationResource>(resourceName);
	}

	AnimationPlayer::~AnimationPlayer()
	{
		Release();
	}

	void AnimationPlayer::Release()
	{
		m_Time = 0.0;
		m_Bones.reset();
	}

	bool AnimationPlayer::Bind(CCC::Resources::ModelWithBoneResource* p_Model)
	{
		// ボーンが無ければバインドできない
		if (p_Model->GetBones().empty()) return false;

		// アニメーションリソースにボーンからトラックへのマッピングを作成させる
		if (!mp_AnimationResource->Bind(p_Model))
			return false;

		// モデルのボーン情報を取得
		m_Bones = DirectX::ModelBone::MakeArray(p_Model->GetBoneCount());

		// 総アニメーション時間を計算
		m_Duration = mp_AnimationResource->GetDuration();

		// 正常にバインドができた
		return true;
	}

	void AnimationPlayer::Update(float delta)
	{
		m_PrevAnimTime = m_Time;
		m_Time += static_cast<double>(delta);


		double loopTime = mp_AnimationResource->GetDuration();

		// ループした瞬間を検出
		if (m_Time >= loopTime)
		{
			m_Time -= loopTime;
			if (m_PrevAnimTime < loopTime)
				m_IsEnd = true;
		}
		else
			m_IsEnd = false;

	}

	void AnimationPlayer::Apply(CCC::Resources::ModelWithBoneResource* p_Model) const
	{
		const DirectX::Model*  model          = p_Model->GetModel();
		size_t                 nbones         = p_Model->GetBoneCount();
		DirectX::XMMATRIX*     boneTransforms = p_Model->GetDrawBones()->get();
		std::vector<uint32_t>& boneToTrack    = mp_AnimationResource->GetBoneToTrack();

		assert(mp_AnimationResource->IsLoaded());

		if (!nbones || !boneTransforms)
		{
			throw std::invalid_argument("Bone transforms array required");
		}

		if (nbones < model->bones.size())
		{
			throw std::invalid_argument("Bone transforms array is too small");
		}

		if (model->bones.empty())
		{
			throw std::runtime_error("Model is missing bones");
		}

		auto header = reinterpret_cast<const SDKANIMATION_FILE_HEADER*>(mp_AnimationResource->GetData());
		assert(header->Version == SDKMESH_FILE_VERSION);

		// Determine animation time
		auto tick = static_cast<uint32_t>(static_cast<double>(header->AnimationFPS) * m_Time);
		tick %= header->NumAnimationKeys;

		// Compute local bone transforms
		auto frameData = reinterpret_cast<SDKANIMATION_FRAME_DATA*>(mp_AnimationResource->GetData() + header->AnimationDataOffset);

		for (size_t j = 0; j < nbones; ++j)
		{
			if (boneToTrack[j] == DirectX::ModelBone::c_Invalid)
			{
				m_Bones[j] = model->boneMatrices[j];
			}
			else
			{
				auto frame = &frameData[boneToTrack[j]];
				auto data = &frame->pAnimationData[tick];

				DirectX::XMVECTOR quat = DirectX::XMVectorSet(data->Orientation.x, data->Orientation.y, data->Orientation.z, data->Orientation.w);
				if (DirectX::XMVector4Equal(quat, DirectX::g_XMZero))
					quat = DirectX::XMQuaternionIdentity();
				else
					quat = DirectX::XMQuaternionNormalize(quat);

				DirectX::XMMATRIX trans    = DirectX::XMMatrixTranslation(data->Translation.x, data->Translation.y, data->Translation.z);
				DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(quat);
				DirectX::XMMATRIX scale    = DirectX::XMMatrixScaling(data->Scaling.x, data->Scaling.y, data->Scaling.z);

				m_Bones[j] = XMMatrixMultiply(XMMatrixMultiply(rotation, scale), trans);
			}
		}

		// Compute absolute locations
		model->CopyAbsoluteBoneTransforms(nbones, m_Bones.get(), boneTransforms);

		// Adjust for model's bind pose.
		for (size_t j = 0; j < nbones; ++j)
		{
			boneTransforms[j] = XMMatrixMultiply(model->invBindPoseMatrices[j], boneTransforms[j]);
		}
	}

	void AnimationPlayer::GetLocalBoneTransforms(const DirectX::Model& model, size_t nbones, DirectX::XMMATRIX* localBones) const
	{
		assert(mp_AnimationResource->IsLoaded());

		if (!nbones || !localBones)
			throw std::invalid_argument("Bone transforms array required");

		if (nbones < model.bones.size())
			throw std::invalid_argument("Bone transforms array is too small");

		if (model.bones.empty())
			throw std::runtime_error("Model is missing bones");

		auto header = reinterpret_cast<const SDKANIMATION_FILE_HEADER*>(mp_AnimationResource->GetData());

		// 現在時間のキーインデックス
		auto tick = static_cast<uint32_t>(static_cast<double>(header->AnimationFPS) * m_Time);
		tick %= header->NumAnimationKeys;

		auto frameData = reinterpret_cast<SDKANIMATION_FRAME_DATA*>(mp_AnimationResource->GetData() + header->AnimationDataOffset);

		// ボーンからトラックへのマッピング取得
		std::vector<uint32_t>& boneToTrack = mp_AnimationResource->GetBoneToTrack();


		// 各ボーンのローカル行列を計算（元の Apply() の前半そのまま）
		for (size_t j = 0; j < nbones; ++j)
		{
			if (boneToTrack[j] == DirectX::ModelBone::c_Invalid)
			{
				localBones[j] = model.boneMatrices[j];
			}
			else
			{
				auto frame = &frameData[boneToTrack[j]];
				auto data = &frame->pAnimationData[tick];

				using namespace DirectX;

				XMVECTOR quat = XMVectorSet(data->Orientation.x, data->Orientation.y, data->Orientation.z, data->Orientation.w);
				if (XMVector4Equal(quat, g_XMZero))
					quat = XMQuaternionIdentity();
				else
					quat = XMQuaternionNormalize(quat);

				XMMATRIX trans = XMMatrixTranslation(data->Translation.x, data->Translation.y, data->Translation.z);
				XMMATRIX rotation = XMMatrixRotationQuaternion(quat);
				XMMATRIX scale = XMMatrixScaling(data->Scaling.x, data->Scaling.y, data->Scaling.z);

				// 元コードと同じ順序で合成
				localBones[j] = XMMatrixMultiply(XMMatrixMultiply(rotation, scale), trans);
			}
		}
	}

	void ApplyBlendSDKMESH(const DirectX::Model* model, size_t nbones, const AnimationPlayer* a, const AnimationPlayer* b, float alpha, DirectX::XMMATRIX* boneTransforms)
	{
		using namespace DirectX;

		alpha = std::clamp(alpha, 0.0f, 1.0f);

		std::vector<XMMATRIX> localA(nbones);
		std::vector<XMMATRIX> localB(nbones);
		std::vector<XMMATRIX> localBlend(nbones);

		// それぞれのアニメーションのローカル行列を計算
		a->GetLocalBoneTransforms(*model, nbones, localA.data());
		b->GetLocalBoneTransforms(*model, nbones, localB.data());

		// 各ボーンごとに S/R/T に分解してブレンド
		for (size_t i = 0; i < nbones; ++i)
		{
			XMVECTOR scaleA, rotA, transA;
			XMVECTOR scaleB, rotB, transB;

			XMMatrixDecompose(&scaleA, &rotA, &transA, localA[i]);
			XMMatrixDecompose(&scaleB, &rotB, &transB, localB[i]);

			XMVECTOR scale = XMVectorLerp(scaleA, scaleB, alpha);
			XMVECTOR trans = XMVectorLerp(transA, transB, alpha);
			XMVECTOR rot = XMQuaternionSlerp(rotA, rotB, alpha);

			// S, R, T からローカル行列を再構成
			XMMATRIX mScale = XMMatrixScalingFromVector(scale);
			XMMATRIX mRot = XMMatrixRotationQuaternion(rot);
			XMMATRIX mTrans = XMMatrixTranslationFromVector(trans);

			// 元の順序に合わせる
			localBlend[i] = XMMatrixMultiply(XMMatrixMultiply(mRot, mScale), mTrans);
		}

		// ローカル行列から絶対行列を計算
		model->CopyAbsoluteBoneTransforms(nbones, localBlend.data(), boneTransforms);

		// バインドポーズ補正
		for (size_t j = 0; j < nbones; ++j)
		{
			boneTransforms[j] = XMMatrixMultiply(model->invBindPoseMatrices[j], boneTransforms[j]);
		}
	}
}

/**
 * @file   AnimationStateBace.h
 *
 * @brief  アニメーション状態基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/10
 * 2025/12/10
 * 作成
 * 
 * 2026/01/10
 * アニメーションの切り替え関数の改修
 */

#include "pch.h"
#include "AnimationStateBace.h"

#include <CCC/Managers/ResourceManager.h>

#include <CCC/ResourceClasses/ModelWithBoneResource.h>

namespace CCC::Animations
{
	AnimationStateBace::AnimationStateBace(const std::string& modelName, const std::string& firstName) :
		m_AnimationSpeed(1.0f),
		m_BlendTime(0.0f),
		m_CurrentBlendTime(0.0f),
		mp_Model(nullptr),
		m_CurrentAnimationName(firstName)
	{
		// リソースマネージャーの取得
		CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();
		// モデルリソースの取得
		mp_Model = p_rm->GetResource<CCC::Resources::ModelWithBoneResource>(modelName);
	}

	AnimationStateBace::~AnimationStateBace()
	{
		Finalize();
	}

	void AnimationStateBace::Update(float elapsedTime)
	{
		// 現在のアニメーションを更新
		m_AnimationPlayers[m_CurrentAnimationName]->Update(elapsedTime * m_AnimationSpeed);

		// ブレンドアニメーションが存在すれば更新
		if (!m_BlendToAnimationName.empty())
		{
			m_AnimationPlayers[m_BlendToAnimationName]->Update(elapsedTime * m_AnimationSpeed);
			m_CurrentBlendTime += elapsedTime * m_AnimationSpeed;
			if (m_CurrentBlendTime >= m_BlendTime)
			{
				// ブレンド終了
				m_AnimationPlayers[m_CurrentAnimationName]->ResetTime();
				m_CurrentAnimationName = m_BlendToAnimationName;
				m_BlendToAnimationName.clear();
				m_BlendTime = 0.0f;
			}
		}
	}

	void AnimationStateBace::RequestAnimationChange(const std::string& animationName, float blendTime)
	{
		// すでに同じアニメーションなら何もしない
		if (m_CurrentAnimationName == animationName) return;
		// ブレンドするアニメーションが同じアニメーションなら何もしない
		if (m_BlendToAnimationName == animationName) return;

		// ブレンド時間が0以下なら即座に変更
		if (blendTime <= 0.0f)
		{
			m_CurrentAnimationName = animationName;
			m_BlendToAnimationName.clear();
			m_BlendTime = 0.0f;

			return;
		}

		// 既にブレンド中の場合はそのブレンドを現在のアニメーションとして扱う
		if (!m_BlendToAnimationName.empty())
		{
			m_CurrentAnimationName = m_BlendToAnimationName;
		}

		// ブレンドアニメーションを設定
		m_BlendToAnimationName = animationName;
		m_BlendTime = blendTime;
		m_CurrentBlendTime = 0.0f;
	}

	void AnimationStateBace::Render(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view)
	{
		CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();

		auto context = p_rm->GetD3DDeviceContext();
		auto states = p_rm->GetCommonStates();

		// ブレンドアニメーションが存在しなければ通常描画
		if (m_BlendToAnimationName.empty())
		{
			m_AnimationPlayers[m_CurrentAnimationName]->Apply(mp_Model);

			// モデルの描画
			mp_Model->GetModel()->DrawSkinned(
				context, *states,
				mp_Model->GetBoneCount(),
				mp_Model->GetDrawBones()->get(),
				world, view,
				p_rm->GetProjection());
		}
		else
		{
			std::vector<DirectX::XMMATRIX> boneTransforms(mp_Model->GetBoneCount());

			CCC::Animations::ApplyBlendSDKMESH(mp_Model->GetModel(), mp_Model->GetBoneCount(),
				m_AnimationPlayers[m_CurrentAnimationName].get(), m_AnimationPlayers[m_BlendToAnimationName].get(),
				std::min(m_CurrentBlendTime / m_BlendTime, 1.0f),
				boneTransforms.data());

			// モデルの描画
			mp_Model->GetModel()->DrawSkinned(
				context, *states,
				mp_Model->GetBoneCount(),
				boneTransforms.data(),
				world, view,
				p_rm->GetProjection());
		}
	}

	void AnimationStateBace::AddAnimationPlayer(const std::string& animationName)
	{
		m_AnimationPlayers.emplace(animationName,
			std::make_unique<CCC::Animations::AnimationPlayer>(animationName));

		m_AnimationPlayers[animationName]->Bind(mp_Model);
	}

	void AnimationStateBace::Finalize()
	{
		m_AnimationPlayers.clear();
	}
}

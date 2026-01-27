/**
 * @file   SoundEffectResource.cpp
 *
 * @brief   サウンドエフェクトリソースのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/27
 * サウンドエフェクトを保持するクラス
 *
 * 2026/01/27
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "SoundEffectResource.h"

// 管理クラス
#include <CCC/Managers/AudioManager.h>

namespace CCC::Resources
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	SoundEffectResource::SoundEffectResource(const wchar_t* path, const std::string& useScene) :
		ResourceBase(path, useScene)
	{
		// リソースの作成
		m_Resource = std::make_unique<DirectX::SoundEffect>(
			CCC::Managers::AudioManager::GetInstance()->GetAudioEngine(),
			path
		);
	}

	SoundEffectResource::~SoundEffectResource() = default;
	
	void SoundEffectResource::Load()
	{
		// リソースの実体の作成
		m_ResourceInstance = m_Resource->CreateInstance();
	}

	void SoundEffectResource::Unload()
	{
		// 実体の破棄
		m_ResourceInstance.reset();
	}

	DirectX::SoundEffectInstance* SoundEffectResource::GetResource()
	{
		return m_ResourceInstance.get();
	}
}

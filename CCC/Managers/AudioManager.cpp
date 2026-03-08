/**
 * @file   AudioManager.cpp
 *
 * @brief  音の管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/27
 * 音に関する再生を管理するクラス
 *
 * 2026/01/27
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "AudioManager.h"

// 管理クラス
#include <CCC/Managers/ResourceManager.h>

// リソースクラス
#include <CCC/ResourceClasses/SoundEffectResource.h>

// メッセンジャー
#include <CCC/Messenger/MessageType.h>
#include <CCC/Messenger/MessengerHub.h>

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<AudioManager> AudioManager::s_Instance = nullptr;

	AudioManager::AudioManager()
	{
		using namespace DirectX;

		// ---------------------------------------------------------------------- //
		// 初期化処理
		// ---------------------------------------------------------------------- //
		
		// エンジン
		AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
		eflags |= AudioEngine_Debug;
#endif
		m_AudioEngine = std::make_unique<AudioEngine>(eflags);



		// ---------------------------------------------------------------------- //
		// メッセージの作成
		// ---------------------------------------------------------------------- //

		// メッセージハブの取得
		CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();

		// 再生のメッセージを作成
		p_mh->Subscribe(
			CCC::Messenger::MessageType::SE_PLAY_START_BUTTOM,
			[](const CCC::Messenger::MessengerHub::PayLoad&) {
				CCC::Managers::ResourceManager::GetInstance()->
					GetResource<CCC::Resources::SoundEffectResource>("SE_StartButtom")->GetResource()->Play();
			}
		);
		p_mh->Subscribe(
			CCC::Messenger::MessageType::BGM_PLAY_BATTLE_01,
			[](const CCC::Messenger::MessengerHub::PayLoad&) {
				CCC::Managers::ResourceManager::GetInstance()->
					GetResource<CCC::Resources::SoundEffectResource>("BGM_Battle01")->GetResource()->Play(true);
			}
		);

		// 停止のメッセージを作成
		p_mh->Subscribe(
			CCC::Messenger::MessageType::BGM_STOP_BATTLE_01,
			[](const CCC::Messenger::MessengerHub::PayLoad&) {
				CCC::Managers::ResourceManager::GetInstance()->
					GetResource<CCC::Resources::SoundEffectResource>("BGM_Battle01")->GetResource()->Stop();
			}
		);

		m_AudioEngine->SetMasterVolume(0.0f);
	}

	AudioManager* AudioManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new AudioManager());

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	AudioManager::~AudioManager() = default;
	
	void AudioManager::Update()
	{
		// エンジンの更新
		if (!m_AudioEngine->Update())
		{
			// アクティブなオーディオデバイスがない
			if (m_AudioEngine->IsCriticalError())
			{
				// TODO:スピーカーが接続されていない場合の処理を追加
				// TODO:上記以外の場合の処理
			}
		}
	}

	DirectX::AudioEngine* AudioManager::GetAudioEngine()
	{
		return m_AudioEngine.get();
	}
}

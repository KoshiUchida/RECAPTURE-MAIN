/**
 * @file   AudioManager.h
 *
 * @brief  音の管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/27
 * 音に関する再生を管理するクラス
 * 
 * 2026/01/27
 * 作成
 */

 // 多重インクルードガード
#pragma once

// C++標準ライブラリ
#include <memory>

// DirectXTK
#include <Audio.h>

namespace CCC::Managers
{
	/// <summary>
	/// オーディオ管理クラス
	/// </summary>
	class AudioManager final
	{
		// ---------------------------------------------------------------------- //
		// シングルトンパターン関連
		// ---------------------------------------------------------------------- //
	private:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		AudioManager();

		// このクラスの実体
		static std::unique_ptr<AudioManager> s_Instance;


	public:
		/// <summary>
		/// 実体の取得
		/// </summary>
		/// <returns>リソース管理クラスへのポインタ</returns>
		static AudioManager* GetInstance();



		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// デストラクタ
		/// </summary>
		~AudioManager();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// オーディオエンジンの取得
		/// </summary>
		DirectX::AudioEngine* GetAudioEngine();



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		// オーディオエンジン
		std::unique_ptr<DirectX::AudioEngine> m_AudioEngine;
	};
}

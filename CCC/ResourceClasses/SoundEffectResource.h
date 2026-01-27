/**
 * @file   SoundEffectResource.h
 *
 * @brief   サウンドエフェクトリソースのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/27
 * サウンドエフェクトを保持するクラス
 * m_UseScene扱うシーン名を保持できるようになっている
 * GetResource関数から間接参照を行うことで再生できる
 *
 * 2026/01/27
 * 作成
 */

 // 多重インクルードガード	
#ifndef SOUND_EFFECT_RESOURCE_DEFINED
#define SOUND_EFFECT_RESOURCE_DEFINED


// 基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

// C++標準ライブラリ
#include <memory>

// DirectXTK
#include <Audio.h>

namespace CCC::Resources
{
	/// <summary>
	/// サウンドエフェクトのリソースクラス
	/// </summary>
	class SoundEffectResource final :
		public Bases::ResourceBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		SoundEffectResource(const wchar_t* path, const std::string& useScene = "");

		/// <summary>
		///  デストラクタ
		/// </summary>
		virtual ~SoundEffectResource();

		/// <summary>
		/// リソースの読み込み
		/// </summary>
		void Load() override;

		/// <summary>
		/// リソースの破棄
		/// </summary>
		void Unload() override;

		/// <summary>
		/// リソースの実体の取得
		/// </summary>
		DirectX::SoundEffectInstance* GetResource();



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		// リソース
		std::unique_ptr<DirectX::SoundEffect> m_Resource;
		// リソースの実体
		std::unique_ptr<DirectX::SoundEffectInstance> m_ResourceInstance;
	};
}

#endif // !SOUND_EFFECT_RESOURCE_DEFINED

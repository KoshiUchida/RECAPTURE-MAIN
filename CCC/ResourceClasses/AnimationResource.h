/**
 * @file   AnimationResource.h
 *
 * @brief  モデルアニメーションリソースクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/08
 */
#pragma once

// 基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

#include <memory>

namespace CCC
{
	namespace Resources
	{
		class AnimationResource final:
			public Bases::ResourceBase
		{
		public:
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
			AnimationResource(const wchar_t* path, const std::string& useScene = "");
			virtual ~AnimationResource();
			void Load() override;
			void Unload() override;

			/// <summary>
			/// リソースデータの解放
			/// </summary>
			void Release() {
				m_Data.reset();
				m_DataSize = 0;
			}

			/// <summary>
			/// バインド
			/// </summary>
			/// <param name="p_ModelResource">モデルリソースへのポインタ</param>
			bool Bind(CCC::Interfaces::IResource* p_ModelResource);



			/// <summary>
			/// リソースが読み込まれているかどうか
			/// </summary>
			bool IsLoaded() const {
				return m_Data != nullptr && m_DataSize > 0;
			}

			/// <summary>
			/// リソースデータへのポインタを取得
			/// </summary>
			uint8_t* GetData() const {
				return m_Data.get();
			}

			/// <summary>
			/// リソースデータのサイズを取得
			/// </summary>
			size_t GetDataSize() const {
				return m_DataSize;
			}

			/// <summary>
			/// アニメーションの総時間を取得
			/// </summary>
			double GetDuration() const;

			/// <summary>
			/// ボーン追跡用の内部ベクター（m_BoneToTrack）への参照を取得
			/// </summary>
			std::vector<uint32_t>& GetBoneToTrack() {
				return m_BoneToTrack;
			}



		private:
			// ---------------------------------------------------------------------- //
			// メンバ関数
			// ---------------------------------------------------------------------- //

			HRESULT PrivLoad();



		private:
			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //

			// アニメーションデータ
			std::unique_ptr<uint8_t[]>  m_Data;
			// アニメーションデータサイズ
			size_t                      m_DataSize;
			// ボーンからトラックへのマッピング
			std::vector<uint32_t>       m_BoneToTrack;
			// バインド済みフラグ
			bool                        m_IsBinded;
		};
	}
}

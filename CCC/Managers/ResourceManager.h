/**
 * @file   ResourceManager.h
 *
 * @brief  リソース管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/01/05
 * 
 * リソースを管理するためのクラス
 * 主な機能はリソースの読み込み、破棄、提供である。
 * 必ず、SetResources関数を使いGameクラスで
 * 必要なリソースをセットしなければいけない
 * 
 * 2025/01/05
 * StepTimerへのポインタを保持するようになった
 * リソース設定関数に必須な引値、StepTimerを追加
 */

#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED

#include "DeviceResources.h"
#include "StepTimer.h"

#include <CCC/Interfaces/IResource.h>

 // 前方宣言
class Game;

namespace CCC
{
	namespace Managers
	{
		class ResourceManager final
		{
			// ---------------------------------------------------------------------- //
			// シングルトンパターン関連
			// ---------------------------------------------------------------------- //
		private:
			// 実体を他のクラスが作れないように

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ResourceManager();

			// このクラスの実体
			static std::unique_ptr<ResourceManager> s_Instance;


		public:
			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns>リソース管理クラスへのポインタ</returns>
			static ResourceManager* GetInstance();



			// ---------------------------------------------------------------------- //
			// メンバ関数
			// ---------------------------------------------------------------------- //
		private:
			/// <summary>
			/// リソースの生成
			/// リソースは必ずここで登録をしなければならない
			/// </summary>
			void MakeResources();



			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			~ResourceManager();

			/// <summary>
			/// リソースのセット
			/// </summary>
			/// <param name="p_DR">デバイスリソースへのポインタ</param>
			/// <param name="p_Timer">ステップタイマーへのポインタ</param>
			void SetResources(DX::DeviceResources* p_DR, DX::StepTimer* p_Timer);

			/// <summary>
			/// リソースの読み込み
			/// </summary>
			void LoadResources(const std::string& scene = "");

			/// <summary>
			/// リソースの破棄
			/// </summary>
			void DeleteResources(const std::string& scene = "");



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// デバイスリソース
			DX::DeviceResources* mp_DeviceResources;

			// 共通ステート
			std::unique_ptr<DirectX::CommonStates> m_CommonStates;

			// 射影行列
			DirectX::SimpleMath::Matrix m_Proj;

			// スプライトバッチ
			std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

			// 描画ループタイマー
			DX::StepTimer* mp_Timer;



			// ---------------------------------------------------------------------- //
			// リソース関連
			// ---------------------------------------------------------------------- //

			// リソースの共通コンテナ
			using ResourceCollection = std::unordered_map<std::string, std::unique_ptr<CCC::Interfaces::IResource>>;
			ResourceCollection m_Resources;

			/// <summary>
			/// リソースの生成
			/// </summary>
			template<class T, class... Args>
			void CreateResource(const std::string& name, Args&&... args)
			{
				auto resource = std::make_unique<T>(std::forward<Args>(args)...);
				m_Resources.emplace(name, std::move(resource));
			}

		public:

			/// <summary>
			/// リソースの取得
			/// </summary>
			template<class T>
			T* GetResource(const std::string& name)
			{
				auto it = m_Resources.find(name);
				if (it == m_Resources.end())
					return nullptr;

				return dynamic_cast<T*>(it->second.get());
			}




			// ---------------------------------------------------------------------- //
			// 取得関数
			// ---------------------------------------------------------------------- //
		public:

			/// <summary>
			/// デバイスリソースへのポインタを取得する
			/// </summary>
			DX::DeviceResources* GetDeviceResources() { return mp_DeviceResources; }

			/// <summary>
			/// デバイスの取得
			/// </summary>
			ID3D11Device1* GetD3DDevice() const noexcept { return mp_DeviceResources->GetD3DDevice(); }

			/// <summary>
			/// デバイスコンテキストの取得
			/// </summary>
			ID3D11DeviceContext1* GetD3DDeviceContext() const noexcept { return mp_DeviceResources->GetD3DDeviceContext(); }

			/// <summary>
			/// 共通ステートの取得
			/// </summary>
			DirectX::CommonStates* GetCommonStates() { return m_CommonStates.get(); }

			/// <summary>
			/// 斜影行列の取得
			/// </summary>
			const DirectX::SimpleMath::Matrix GetProjection() const { return m_Proj; }

			/// <summary>
			/// スプライトバッチの取得
			/// </summary>
			DirectX::SpriteBatch* GetSpriteBatch() { return m_SpriteBatch.get(); }

			/// <summary>
			/// ステップタイマーの取得
			/// </summary>
			DX::StepTimer* GetStepTimer() { return mp_Timer; }
		};
	}
}

#endif // !RESOURCE_MANAGER_DEFINED

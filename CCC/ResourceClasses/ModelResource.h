/**
 * @file   ModelResource.h
 *
 * @brief  モデルリソースのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/11/16
 * モデルを保持するクラス
 */

#pragma once
#ifndef MODEL_RESOURCE_DEFINED
#define MODEL_RESOURCE_DEFINED

// 基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

namespace CCC
{
	namespace Resources
	{
		/// <summary>
		/// モデルのリソースクラス
		/// </summary>
		class ModelResource final:
			public Bases::ResourceBase
		{
		public:
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ModelResource(const wchar_t* path, const std::string& useScene = "");

			/// <summary>
			///  デストラクタ
			/// </summary>
			virtual ~ModelResource();

			/// <summary>
			/// リソースの読み込み
			/// </summary>
			void Load() override;

			/// <summary>
			/// リソースの破棄
			/// </summary>
			void Unload() override;

			/// <summary>
			/// モデルへのポインタの取得
			/// </summary>
			/// <returns>モデルへのポインタ</returns>
			DirectX::Model* GetModel() { return m_Model.get(); }



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// モデル（リソース）
			std::unique_ptr<DirectX::Model> m_Model;
		};
	}
}

#endif // !MODEL_RESOURCE_DEFINED

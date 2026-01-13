/**
 * @file   ModelWithBoneResource.h
 *
 * @brief  モデル(ボーン付き)リソースクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/10
 * 
 * ボーンがついているモデルリソースのクラス
 */

#pragma once
#include <CCC/ResourceClasses/ResourceBase.h>

namespace CCC
{
	namespace Resources
	{
		// 前方宣言
		class ModelWithBoneResource;

		/// <summary>
		/// モデル（ボーン付き）リソースクラス
		/// </summary>
		class ModelWithBoneResource final:
			public CCC::Bases::ResourceBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			ModelWithBoneResource(const wchar_t* path, const std::string& useScene = "");

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~ModelWithBoneResource();

			/// <summary>
			/// リソースの読み込み
			/// </summary>
			void Load() override;

			/// <summary>
			/// リソースの破棄
			/// </summary>
			void Unload() override;

			/// <summary>
			/// モデルの取得
			/// </summary>
			/// <returns>モデルへのポインタ</returns>
			DirectX::Model* GetModel() { return m_Model.get(); }

			/// <summary>
			/// モデル内のボーンの数を返します。
			/// </summary>
			/// <returns>モデルが保持するボーンの数</returns>
			size_t GetBoneCount() const {
				return m_Model->bones.size();
			}

			/// <summary>
			/// ボーンへの変換行列配列の取得
			/// </summary>
			/// <returns>ボーンへの変換行列配列</returns>
			DirectX::ModelBone::TransformArray* GetDrawBones() {
				return &m_DrawBones;
			}

			/// <summary>
			/// 内部モデルが保持するボーン（ModelBone::Collection）への参照を返します。非constの参照のため、呼び出し側でコレクションを変更できます。
			/// </summary>
			/// <returns>DirectX::ModelBone::Collection への参照。内部メンバ m_Model->bones を指します。</returns>
			DirectX::ModelBone::Collection& GetBones() {
				return m_Model->bones;
			}

			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// モデル（リソース）
			std::unique_ptr<DirectX::Model> m_Model;

			// モデルのボーン
			DirectX::ModelBone::TransformArray m_DrawBones;

			// バインド済みかどうか
			bool m_IsBinded;
		};
	}

}


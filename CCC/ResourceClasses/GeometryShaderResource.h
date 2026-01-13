/**
 * @file   GeometryShaderResource.h
 *
 * @brief  ジオメトリシェーダリソースのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/09
 * ジオメトリシェーダを保持するクラス
 */

#pragma once

// リソースの基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

namespace CCC::Resources
{
	/// <summary>
	/// ジオメトリシェーダリソースクラス
	/// </summary>
	class GeometryShaderResource final :
		public CCC::Bases::ResourceBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GeometryShaderResource(const wchar_t* path, const std::string& useScene = "");

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~GeometryShaderResource();

		/// <summary>
		/// リソースの読み込み
		/// </summary>
		void Load() override;

		/// <summary>
		/// リソースの破棄
		/// </summary>
		void Unload() override;


		ID3D11GeometryShader** GetAddressOf();

		ID3D11GeometryShader* Get();



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_Shader;
	};
}


/**
 * @file   VertexShaderResource.h
 *
 * @brief  頂点シェーダリソースのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/09
 * 頂点シェーダを保持するクラス
 */

#pragma once

#include <string>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcommon.h>

// リソースの基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

namespace CCC::Resources
{
	/// <summary>
	/// 頂点シェーダリソースクラス
	/// </summary>
	class VertexShaderResource final :
		public CCC::Bases::ResourceBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		VertexShaderResource(const wchar_t* path, const std::string& useScene = "");

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~VertexShaderResource();

		/// <summary>
		/// リソースの読み込み
		/// </summary>
		void Load() override;

		/// <summary>
		/// リソースの破棄
		/// </summary>
		void Unload() override;


		ID3D11VertexShader** GetAddressOf();
		ID3D11VertexShader* Get();

		const void* GetBytecodePtr()  const { return m_Bytecode ? m_Bytecode->GetBufferPointer() : nullptr; }
		size_t      GetBytecodeSize() const { return m_Bytecode ? m_Bytecode->GetBufferSize() : 0; }



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_Shader;	// リソース

		Microsoft::WRL::ComPtr<ID3DBlob         > m_Bytecode;	// VSバイトコード
	};
}

/**
 * @file   PixelShaderResource.h
 *
 * @brief  ピクセルシェーダリソースのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/12
 * ピクセルシェーダを保持するクラス
 * 
 * 2026/01/09
 * 作成
 * 
 * 2026/01/12
 * データサイズを取得できるようにコンパイル済バイトコードを保持するように修正
 */

#pragma once

// リソースの基底クラス
#include <CCC/ResourceClasses/ResourceBase.h>

namespace CCC::Resources
{
	/// <summary>
	/// ピクセルシェーダリソースクラス
	/// </summary>
	class PixelShaderResource final :
		public CCC::Bases::ResourceBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		PixelShaderResource(const wchar_t* path, const std::string& useScene = "");

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~PixelShaderResource();

		/// <summary>
		/// リソースの読み込み
		/// </summary>
		void Load() override;

		/// <summary>
		/// リソースの破棄
		/// </summary>
		void Unload() override;


		ID3D11PixelShader** GetAddressOf();
		ID3D11PixelShader* Get();

		const void* GetBytecodePtr()  const { return m_Bytecode ? m_Bytecode->GetBufferPointer() : nullptr; }
		size_t      GetBytecodeSize() const { return m_Bytecode ? m_Bytecode->GetBufferSize() : 0; }



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_Shader;		// リソース

		Microsoft::WRL::ComPtr<ID3DBlob         > m_Bytecode;	// PSバイトコード
	};
}

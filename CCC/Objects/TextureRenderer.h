/**
 * @file   TextureRenderer.h
 *
 * @brief  画像を描画するオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/19
 * 画像を指定の画面上に描画するだけのオブジェクト
 *
 * 2026/01/19
 * 作成
 */

 // 多重インクルードガード
#pragma once

// 親クラス
#include "ObjectBase.h"


// C++標準ライブラリ
#include <string>
#include <memory>
#include <wrl/client.h>

// DirectX11
#include <d3d11.h>

// DirectXTK
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>


namespace CCC
{
	// 前方宣言
	namespace Resources
	{
		class TextureResource;
		class PixelShaderResource;
		class GeometryShaderResource;
		class VertexShaderResource;
	}
	namespace Managers
	{
		class ResourceManager;
	}


	namespace Bases
	{
		/// <summary>
		/// テクスチャ描画オブジェクトクラス
		/// </summary>
		class TextureRenderer :
			public ObjectBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="position">画面上の表示位置</param>
			/// <param name="size">サイズ</param>
			/// <param name="texture">テクスチャリソースの登録名</param>
			TextureRenderer(
				const DirectX::SimpleMath::Vector2& position,
				float                               size,
				const std::string&                  texture
			);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~TextureRenderer();

			/// <summary>
			/// 初期化処理
			/// </summary>
			void Initialize() override final;

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime) override final;

			/// <summary>
			/// 描画処理
			/// </summary>
			void Render() override final;

			/// <summary>
			/// 終了処理
			/// </summary>
			void Finalize() override final;



			// ---------------------------------------------------------------------- //
			// 描画関連
			// ---------------------------------------------------------------------- //
		public:
			// 定数バッファ
			struct ConstBuffer
			{
				DirectX::SimpleMath::Matrix  matWorld;
				DirectX::SimpleMath::Matrix  matView;
				DirectX::SimpleMath::Matrix  matProjection;
				DirectX::SimpleMath::Vector4 screenAndTextureSize;
				DirectX::SimpleMath::Vector4 diffuse;
			};

			// インプットレイアウト
			static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

		private:
			// 管理クラス
			CCC::Managers::ResourceManager* mp_ResourceManager;	// リソース管理クラス

			// テクスチャリソースクラス
			CCC::Resources::TextureResource* mp_Default;	// 何も操作がされていない時のテクスチャ

			// シェーダリソース
			CCC::Resources::PixelShaderResource* mp_DefaultPS;
			CCC::Resources::VertexShaderResource* mp_DefaultVS;
			CCC::Resources::GeometryShaderResource* mp_DisplayGS;

			// ボタンピクセルシェーダ用の定数バッファ
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_ButtomBuffer;

			//	プリミティブバッチ
			std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;

			//	入力レイアウト
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			DirectX::SimpleMath::Vector2  m_Position;		// 画面上の座標
			float                         m_Size;			// サイズ
			DirectX::SimpleMath::Vector2  m_TextureSize;	// 画像のサイズ
		};
	}
}

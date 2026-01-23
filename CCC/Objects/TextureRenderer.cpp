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

 // プリコンパイル済みヘッダー	
#include "pch.h"

// クラス定義元
#include "TextureRenderer.h"


// 画面情報
#include <Main/DisplayInfo.h>

// リソースクラス
#include <CCC/ResourceClasses/TextureResource.h>
#include <CCC/ResourceClasses/PixelShaderResource.h>
#include <CCC/ResourceClasses/GeometryShaderResource.h>
#include <CCC/ResourceClasses/VertexShaderResource.h>

// 管理クラス
#include <CCC/Managers/ResourceManager.h>


namespace CCC::Bases
{
	///	<summary>
	///	インプットレイアウト
	///	</summary>
	const std::vector<D3D11_INPUT_ELEMENT_DESC> TextureRenderer::INPUT_LAYOUT =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	TextureRenderer::TextureRenderer(
		const DirectX::SimpleMath::Vector2& position,
		float size,
		const std::string& texture
	) :
		ObjectBase(),
		m_Position(position),
		m_Size(size),
		m_TextureSize(DirectX::SimpleMath::Vector2::Zero),

		mp_ResourceManager(CCC::Managers::ResourceManager::GetInstance()),
		mp_Default(nullptr),
		mp_DefaultPS(nullptr),
		mp_DefaultVS(nullptr),
		mp_DisplayGS(nullptr)
	{
		// リソースの取得
		mp_Default   = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >(texture);
		mp_DefaultPS = mp_ResourceManager->GetResource<CCC::Resources::PixelShaderResource   >("DefaultPS");
		mp_DefaultVS = mp_ResourceManager->GetResource<CCC::Resources::VertexShaderResource  >("DefaultVS");
		mp_DisplayGS = mp_ResourceManager->GetResource<CCC::Resources::GeometryShaderResource>("DisplayGS");
	}

	TextureRenderer::~TextureRenderer() = default;

	void TextureRenderer::Initialize()
	{
		// デバイスの取得
		auto device = mp_ResourceManager->GetD3DDevice();

		// テクスチャのサイズを取得
		m_TextureSize =
			DirectX::SimpleMath::Vector2(static_cast<float>(mp_Default->GetWidth()), static_cast<float>(mp_Default->GetHeight())) * m_Size;

		//	プリミティブバッチの作成
		m_PrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(mp_ResourceManager->GetD3DDeviceContext());



		// ---------------------------------------------------------------------- //
		// 定数バッファの作成
		// ---------------------------------------------------------------------- //

		D3D11_BUFFER_DESC desc{};
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(ConstBuffer);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		device->CreateBuffer(&desc, nullptr, &m_ButtomBuffer);


		//	インプットレイアウトの作成
		device->CreateInputLayout(&INPUT_LAYOUT[0],
			static_cast<UINT>(INPUT_LAYOUT.size()),
			mp_DefaultVS->GetBytecodePtr(), mp_DefaultVS->GetBytecodeSize(),
			m_InputLayout.GetAddressOf());
	}

	void TextureRenderer::Update(float)
	{
	}

	void TextureRenderer::Render()
	{
		// 取得
		ID3D11DeviceContext1* context = mp_ResourceManager->GetD3DDeviceContext();	// コンテキストの取得
		DirectX::DX11::CommonStates* state = mp_ResourceManager->GetCommonStates();	// コモンステートの取得

		// ---------------------------------------------------------------------- //
		// 定数バッファの更新
		// ---------------------------------------------------------------------- //

		//	頂点情報(板ポリゴンの４頂点の座標情報）
		DirectX::DX11::VertexPositionColorTexture vertex[1] =
		{
			// ゲージの表示
			DirectX::DX11::VertexPositionColorTexture(
				DirectX::SimpleMath::Vector3(m_Position.x, m_Position.y, 0.0f),
				DirectX::Colors::Black,
				DirectX::SimpleMath::Vector2::Zero
			),
		};

		//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
		ConstBuffer cbuff;
		cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
		cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
		cbuff.matProjection = DirectX::SimpleMath::Matrix::Identity;
		cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
			DisplayInfo::Width,
			DisplayInfo::Height,
			m_TextureSize.x,
			m_TextureSize.y
		);
		cbuff.diffuse = DirectX::SimpleMath::Vector4::One;

		//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
		context->UpdateSubresource(m_ButtomBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//	シェーダーにバッファを渡す
		ID3D11Buffer* cb[1] = { m_ButtomBuffer.Get() };
		context->VSSetConstantBuffers(0, 1, cb);
		context->GSSetConstantBuffers(0, 1, cb);
		context->PSSetConstantBuffers(0, 1, cb);

		//	画像用サンプラーの登録
		ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
		context->PSSetSamplers(0, 1, sampler);

		//	半透明描画指定
		ID3D11BlendState* blendstate = state->NonPremultiplied();

		//	透明判定処理
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		//	深度バッファに書き込み参照する
		context->OMSetDepthStencilState(state->DepthDefault(), 0);

		//	カリングは左周り
		context->RSSetState(state->CullNone());

		//	シェーダをセットする
		context->VSSetShader(mp_DefaultVS->Get(), nullptr, 0);
		context->GSSetShader(mp_DisplayGS->Get(), nullptr, 0);
		context->PSSetShader(mp_DefaultPS->Get(), nullptr, 0);

		context->PSSetShaderResources(0, 1, mp_Default->GetAddressOf());

		//	インプットレイアウトの登録
		context->IASetInputLayout(m_InputLayout.Get());

		//	板ポリゴンを描画
		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
		m_PrimitiveBatch->End();


		//	シェーダの登録を解除しておく
		context->VSSetShader(nullptr, nullptr, 0);
		context->GSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);
	}

	void TextureRenderer::Finalize()
	{
	}
}

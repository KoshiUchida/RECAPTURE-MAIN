// プリコンパイル済みヘッダー	
#include "pch.h"

// クラス定義元
#include "ButtomUIBase.h"


// 画面情報
#include <Main/DisplayInfo.h>

// リソースクラス
#include <CCC/ResourceClasses/TextureResource.h>
#include <CCC/ResourceClasses/PixelShaderResource.h>
#include <CCC/ResourceClasses/GeometryShaderResource.h>
#include <CCC/ResourceClasses/VertexShaderResource.h>

// 管理クラス
#include <CCC/Managers/ResourceManager.h>

// メッセンジャー
#include <CCC/Messenger/MessengerHub.h>


namespace CCC::Bases
{
	///	<summary>
	///	インプットレイアウト
	///	</summary>
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ButtomUIBase::INPUT_LAYOUT =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	ButtomUIBase::ButtomUIBase(
		const DirectX::SimpleMath::Vector2& position,
		float                               size,
		const std::string&                  texture,
		const CCC::Messenger::MessageType& pressedMessage,
		const CCC::Messenger::MessageType& pushingMessage,
		const CCC::Messenger::MessageType& releasedMessage
	) :
		ObjectBase(),
		m_Position(position),
		m_Size(size),
		m_TextureSize(DirectX::SimpleMath::Vector2::Zero),
		m_IsPushing(false),
		m_RequestPress(false),

		mp_ResourceManager(CCC::Managers::ResourceManager::GetInstance()),
		mp_Default  (nullptr),
		mp_DefaultPS(nullptr),
		mp_DefaultVS(nullptr),
		mp_DisplayGS(nullptr),

		mp_MessengerHub(CCC::Messenger::MessengerHub::GetInstance()),
		m_PressedMessege(pressedMessage),
		m_PushingMessege(pushingMessage),
		m_ReleasedMessege(releasedMessage)
	{
		// リソースの取得
		mp_Default   = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >(texture);
		mp_DefaultPS = mp_ResourceManager->GetResource<CCC::Resources::PixelShaderResource   >("DefaultPS");
		mp_DefaultVS = mp_ResourceManager->GetResource<CCC::Resources::VertexShaderResource  >("DefaultVS");
		mp_DisplayGS = mp_ResourceManager->GetResource<CCC::Resources::GeometryShaderResource>("DisplayGS");
	}

	ButtomUIBase::~ButtomUIBase() = default;
	
	void ButtomUIBase::Initialize()
	{
		// テクスチャのサイズを取得
		m_TextureSize = DirectX::SimpleMath::Vector2(mp_Default->GetWidth(), mp_Default->GetHeight()) * m_Size;



		// デバイスの取得
		auto device = mp_ResourceManager->GetD3DDevice();


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


		// ---------------------------------------------------------------------- //
		// メッセンジャーに登録
		// ---------------------------------------------------------------------- //

		mp_MessengerHub->Subscribe(CCC::Messenger::MessageType::INPUT_A,
			[this](const CCC::Messenger::MessengerHub::PayLoad& payload)
			{
				// 取得する値が正常かを確かめる
				if (const DirectX::SimpleMath::Vector2* p = std::any_cast<DirectX::SimpleMath::Vector2>(&payload.item))
				{
					// BBOの４点を計算
					float top = m_Position.y - m_TextureSize.y * 0.5f;
					float bottom = m_Position.y + m_TextureSize.y * 0.5f;
					float left = m_Position.x - m_TextureSize.x * 0.5f;
					float right = m_Position.x + m_TextureSize.x * 0.5f;

					// 入力があった画面上の位置と重なるか
					if (top <= p->y && p->y <= bottom && left <= p->x && p->x <= right)
						// 重なった場合ボタンを押す
						this->Press();
				}

			});
	}
	
	void ButtomUIBase::Update(float elapsedTime)
	{
		// 警告回避
		elapsedTime;

		if (m_RequestPress && !m_IsPushing)
		{
			m_IsPushing = true;

			if (m_PressedMessege != CCC::Messenger::MessageType::NONE)
				mp_MessengerHub->Receive(
					m_PressedMessege,
					CCC::Messenger::MessengerHub::PayLoad(true)
				);
		}
		else if (m_RequestPress && m_IsPushing)
		{
			m_IsPushing = true;
			
			if (m_PushingMessege != CCC::Messenger::MessageType::NONE)
				mp_MessengerHub->Receive(
					m_PushingMessege,
					CCC::Messenger::MessengerHub::PayLoad(true)
				);
		}
		else if (!m_RequestPress && m_IsPushing)
		{
			m_IsPushing = false;

			if (m_ReleasedMessege != CCC::Messenger::MessageType::NONE)
				mp_MessengerHub->Receive(
					m_ReleasedMessege,
					CCC::Messenger::MessengerHub::PayLoad(true)
				);
		}

		m_RequestPress = false;
	}
	
	void ButtomUIBase::Render()
	{
		// 取得
		ID3D11DeviceContext1*        context = mp_ResourceManager->GetD3DDeviceContext();	// コンテキストの取得
		DirectX::DX11::CommonStates* state   = mp_ResourceManager->GetCommonStates();	// コモンステートの取得

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
		cbuff.matWorld             = DirectX::SimpleMath::Matrix::Identity;
		cbuff.matView              = DirectX::SimpleMath::Matrix::Identity;
		cbuff.matProjection        = DirectX::SimpleMath::Matrix::Identity;
		cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
			DisplayInfo::Width,
			DisplayInfo::Height,
			m_TextureSize.x,
			m_TextureSize.y
		);
		cbuff.diffuse              = DirectX::SimpleMath::Vector4::One;

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
	
	void ButtomUIBase::Finalize()
	{
		// メッセンジャーにてINPUT_Aの関数をリセットする
		CCC::Messenger::MessengerHub::GetInstance()->
			Unsubscribe(CCC::Messenger::MessageType::INPUT_A);
	}

	void ButtomUIBase::Press()
	{
		m_RequestPress = true;
	}
}

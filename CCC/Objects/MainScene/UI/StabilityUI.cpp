// 01/06
// 安定度の状態をターゲットから参照するように修正

// プリコンパイル済みヘッダー	
#include "pch.h"

// クラス定義元
#include "StabilityUI.h"


// C++標準ライブラリ
#include <cmath>

// Windows API
#include <Windows.h>

// DirectX11
#include <d3d11_1.h>
#include <d3dcommon.h>
#include <dxgiformat.h>

// DirectXTK
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>


// 画面情報
#include <Main/DisplayInfo.h>

// イージング関数
#include <CCC/Common/Easings.h>

// リソースクラス
#include <CCC/ResourceClasses/TextureResource.h>
#include <CCC/ResourceClasses/PixelShaderResource.h>
#include <CCC/ResourceClasses/GeometryShaderResource.h>
#include <CCC/ResourceClasses/VertexShaderResource.h>

// 管理クラス
#include <CCC/Managers/ResourceManager.h>

// 関係のあるオブジェクト
#include <CCC/Objects/MainScene/PawnLeader/PawnLeader.h>

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> StabilityUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

StabilityUI::StabilityUI() :
	mp_Label          ( nullptr ),
	mp_Bar            ( nullptr ),
	mp_Frame          ( nullptr ),
	mp_Warning        ( nullptr ),
	mp_Broken         ( nullptr ),
	mp_GaugePS        ( nullptr ),
	mp_WarningPS      ( nullptr ),
	mp_DefaultPS      ( nullptr ),
	mp_DisplayGS      ( nullptr ),
	mp_DisplayVS      ( nullptr ),
	mp_ResourceManager( CCC::Managers::ResourceManager::GetInstance() ),
	mp_Target         ( nullptr ),
	m_Time            ( 0.0f ),
	m_Stability       ( 0.0f )
{
	// リソースの取得
	mp_Label     = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >("Stability_Label"  );
	mp_Bar       = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >("Stability_Bar"    );
	mp_Frame     = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >("Stability_Frame"  );
	mp_Warning   = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >("Stability_Warning");
	mp_Broken    = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >("Stability_Broken" );
	mp_Death     = mp_ResourceManager->GetResource<CCC::Resources::TextureResource       >("Stability_Death"  );
	mp_GaugePS   = mp_ResourceManager->GetResource<CCC::Resources::PixelShaderResource   >("GaugePS"          );
	mp_WarningPS = mp_ResourceManager->GetResource<CCC::Resources::PixelShaderResource   >("WarningPS"        );
	mp_DefaultPS = mp_ResourceManager->GetResource<CCC::Resources::PixelShaderResource   >("DefaultPS"        );
	mp_DisplayGS = mp_ResourceManager->GetResource<CCC::Resources::GeometryShaderResource>("DisplayGS"        );
	mp_DisplayVS = mp_ResourceManager->GetResource<CCC::Resources::VertexShaderResource  >("DefaultVS"        );
}

StabilityUI::~StabilityUI() = default;

void StabilityUI::Initialize()
{
	// デバイスの取得
	auto device = mp_ResourceManager->GetD3DDevice();


	//	プリミティブバッチの作成
	m_PrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(mp_ResourceManager->GetD3DDeviceContext());

	// ---------------------------------------------------------------------- //
	// 定数バッファの作成
	// ---------------------------------------------------------------------- //

	D3D11_BUFFER_DESC desc{};
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth           = sizeof(ConstBuffer);
	desc.Usage               = D3D11_USAGE_DEFAULT;
	desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags      = 0;
	desc.MiscFlags           = 0;
	desc.StructureByteStride = 0;
	device->CreateBuffer(&desc, nullptr, &m_GaugeBuffer);
	device->CreateBuffer(&desc, nullptr, &m_WarningBuffer);
	device->CreateBuffer(&desc, nullptr, &m_CommonBuffer);


	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		mp_DisplayVS->GetBytecodePtr(), mp_DisplayVS->GetBytecodeSize(),
		m_InputLayout.GetAddressOf());
}

void StabilityUI::Update(float elapsedTime)
{
	// 時間の経過
	m_Time += elapsedTime;
	if (m_Time > 60.0f) m_Time = 0.0f;

	// 陣形安定度の取得
	m_Stability = mp_Target->GetFormationStability();
}

void StabilityUI::Render()
{
	ID3D11DeviceContext1*       context        = mp_ResourceManager->GetD3DDeviceContext();	// コンテキストの取得
	DirectX::DX11::SpriteBatch* spriteBatch    = mp_ResourceManager->GetSpriteBatch();		// スプライトバッチの取得
	PawnLeader::StabilityStates stabilityState = mp_Target->GetStabilityState();			// 安定度の状態を取得

	DirectX::DX11::CommonStates* state = mp_ResourceManager->GetCommonStates();


	// ---------------------------------------------------------------------- //
	// バーの色
	// ---------------------------------------------------------------------- //

	// 安定度の状態でバーの色を変化
	DirectX::XMVECTORF32 barColor;
	switch (stabilityState)
	{
	case PawnLeader::StabilityStates::Stable:
		barColor = DirectX::Colors::Green;
		break;

	case PawnLeader::StabilityStates::Warning:
		barColor = DirectX::Colors::Yellow;
		break;

	case PawnLeader::StabilityStates::Death:
		barColor = DirectX::Colors::Black;
		break;

	case PawnLeader::StabilityStates::Broken:
	case PawnLeader::StabilityStates::Count:
	default:
		barColor = DirectX::Colors::Red;
		break;
	}



	// ---------------------------------------------------------------------- //
	// 定数バッファの更新
	// ---------------------------------------------------------------------- //

	//	頂点情報(板ポリゴンの４頂点の座標情報）
	DirectX::DX11::VertexPositionColorTexture vertex[4] =
	{
		// ゲージの表示
		DirectX::DX11::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(235.0f, DisplayInfo::Height - 45.0f, 0.0f),
			barColor,
			DirectX::SimpleMath::Vector2::Zero
		),
		// 警告・崩壊の表示
		DirectX::DX11::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(225.0f, DisplayInfo::Height - 45.0f, 0.0f),
			barColor,
			DirectX::SimpleMath::Vector2::Zero
		),
		// フレームの表示
		DirectX::DX11::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(235.0f, DisplayInfo::Height - 45.0f, 0.0f),
			DirectX::Colors::Black,
			DirectX::SimpleMath::Vector2::Zero
		),
		// ラベルの表示
		DirectX::DX11::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(135.0f, DisplayInfo::Height - 95.0f, 0.0f),
			DirectX::Colors::Black,
			DirectX::SimpleMath::Vector2::Zero
		),
	};

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matWorld      = DirectX::SimpleMath::Matrix::Identity;
	cbuff.matView       = DirectX::SimpleMath::Matrix::Identity;
	cbuff.matProjection = DirectX::SimpleMath::Matrix::Identity;

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
	context->VSSetShader(mp_DisplayVS->Get(), nullptr, 0);
	context->GSSetShader(mp_DisplayGS->Get(), nullptr, 0);
	
	//	インプットレイアウトの登録
	context->IASetInputLayout(m_InputLayout.Get());




	// ---------------------------------------------------------------------- //
	// 描画
	// ---------------------------------------------------------------------- //

	cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
		DisplayInfo::Width,
		DisplayInfo::Height,
		mp_Bar->GetWidth() * 0.5f,
		mp_Bar->GetHeight() * 0.5f
	);
	cbuff.diffuse = DirectX::SimpleMath::Vector4(m_Stability, m_Time, 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_GaugeBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_GaugeBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	context->PSSetShader(mp_GaugePS->Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, mp_Bar->GetAddressOf());

	//	板ポリゴンを描画
	m_PrimitiveBatch->Begin();
	m_PrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_PrimitiveBatch->End();




	// バー以外の描画
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
		DisplayInfo::Width,
		DisplayInfo::Height,
		mp_Frame->GetWidth() * 0.5f,
		mp_Frame->GetHeight() * 0.5f
	);
	cbuff.diffuse = DirectX::SimpleMath::Vector4::One;

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CommonBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	cb[0] = { m_CommonBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	シェーダをセットする
	context->PSSetShader(mp_DefaultPS->Get(), nullptr, 0);

	// 警告の場合
	context->PSSetShaderResources(0, 1, mp_Frame->GetAddressOf());

	//	板ポリゴンを描画
	m_PrimitiveBatch->Begin();
	m_PrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[2], 1);
	m_PrimitiveBatch->End();


	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
		DisplayInfo::Width,
		DisplayInfo::Height,
		mp_Label->GetWidth() * 0.5f,
		mp_Label->GetHeight() * 0.5f
	);
	cbuff.diffuse = DirectX::SimpleMath::Vector4::One;

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CommonBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	cb[0] = { m_CommonBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 警告の場合
	context->PSSetShaderResources(0, 1, mp_Label->GetAddressOf());

	//	板ポリゴンを描画
	m_PrimitiveBatch->Begin();
	m_PrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[3], 1);
	m_PrimitiveBatch->End();


	

	// 崩壊していたら
	if (stabilityState == PawnLeader::StabilityStates::Broken)
	{
		//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
		cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
			DisplayInfo::Width,
			DisplayInfo::Height,
			mp_Broken->GetWidth() * 0.5f,
			mp_Broken->GetHeight() * 0.5f
		);
		cbuff.diffuse = DirectX::SimpleMath::Vector4(m_Stability, m_Time, 1, 1);

		//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
		context->UpdateSubresource(m_WarningBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//	シェーダーにバッファを渡す
		cb[0] = { m_WarningBuffer.Get() };
		context->VSSetConstantBuffers(0, 1, cb);
		context->GSSetConstantBuffers(0, 1, cb);
		context->PSSetConstantBuffers(0, 1, cb);

		//	シェーダをセットする
		context->PSSetShader(mp_WarningPS->Get(), nullptr, 0);

		// 警告の場合
		context->PSSetShaderResources(0, 1, mp_Broken->GetAddressOf());

		//	板ポリゴンを描画
		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[1], 1);
		m_PrimitiveBatch->End();
	}



	// 警告の場合
	if (stabilityState == PawnLeader::StabilityStates::Warning)
	{
		//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
		cbuff.screenAndTextureSize = DirectX::SimpleMath::Vector4(
			DisplayInfo::Width,
			DisplayInfo::Height,
			mp_Warning->GetWidth() * 0.5f,
			mp_Warning->GetHeight() * 0.5f
		);
		cbuff.diffuse = DirectX::SimpleMath::Vector4(m_Stability, m_Time, 1, 1);

		//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
		context->UpdateSubresource(m_WarningBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//	シェーダーにバッファを渡す
		cb[0] = { m_WarningBuffer.Get() };
		context->VSSetConstantBuffers(0, 1, cb);
		context->GSSetConstantBuffers(0, 1, cb);
		context->PSSetConstantBuffers(0, 1, cb);

		//	シェーダをセットする
		context->PSSetShader(mp_WarningPS->Get(), nullptr, 0);

		// 警告の場合
		context->PSSetShaderResources(0, 1, mp_Warning->GetAddressOf());

		//	板ポリゴンを描画
		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[1], 1);
		m_PrimitiveBatch->End();
	}


	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);






	// 部隊の死の場合
	if (stabilityState == PawnLeader::StabilityStates::Death)
	{
		spriteBatch->Begin();

		//StabilityStateColor.w = 0.3f;

		spriteBatch->Draw(
			mp_Death->Get(),
			DirectX::SimpleMath::Vector2(25.0f, DisplayInfo::Height - 85.0f),
			nullptr,
			DirectX::Colors::Black,
			0.0f,
			DirectX::SimpleMath::Vector2::Zero,
			DirectX::SimpleMath::Vector2(0.5f, 0.5f)
		);

		spriteBatch->End();
	}
}

void StabilityUI::Finalize()
{
}

/**
 * @file   StabilityUI.h
 *
 * @brief  安定度UIオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 *
 * 2026/01/05
 * 作成
 * 
 * 2026/01/06
 * バー上に文字を表示する
 * 
 * 2026/01/10
 * ピクセルシェーダリソースの追加
 * 
 * 2026/01/18
 * コメントの修正
 */

 // 多重インクルードガード
#pragma once

// 親クラス
#include<CCC/Objects/ObjectBase.h>


// C++標準ライブラリ
#include <vector>
#include <memory>
#include <wrl/client.h>

// DirectX11
#include <d3d11.h>

// DirectXTK
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>


// 前方宣言
namespace CCC
{
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
}
class PawnLeader;


class StabilityUI final:
	public CCC::Bases::ObjectBase
{
public:
	StabilityUI();
	~StabilityUI();

	void Initialize() override final;
	void Update(float elapsedTime) override final;
	void Render()override;
	void Finalize() override;

	void SetTarget(PawnLeader* p_Target) {
		mp_Target = p_Target;
	}


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
	// テクスチャリソース
	CCC::Resources::TextureResource* mp_Label;		// ラベル
	CCC::Resources::TextureResource* mp_Frame;		// 枠
	CCC::Resources::TextureResource* mp_Bar;		// バー
	CCC::Resources::TextureResource* mp_Warning;	// バー上の表記：警告
	CCC::Resources::TextureResource* mp_Broken;		// バー上の表記：崩壊
	CCC::Resources::TextureResource* mp_Death;		// バー上の表記：部隊の死

	// シェーダリソース
	CCC::Resources::PixelShaderResource*    mp_GaugePS;		// ゲージ用ピクセルシェーダ
	CCC::Resources::PixelShaderResource*    mp_WarningPS;	// 警告表示用ピクセルシェーダ
	CCC::Resources::GeometryShaderResource* mp_DisplayGS;
	CCC::Resources::VertexShaderResource*   mp_DisplayVS;

	// 管理クラス
	CCC::Managers::ResourceManager* mp_ResourceManager;	// リソース管理クラス

	// ゲージピクセルシェーダ用の定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_GaugeBuffer;

	// Warningピクセルシェーダ用の定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_WarningBuffer;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;

	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	// UIの表示する値を持つオブジェクト
	PawnLeader* mp_Target;

	// 経過時間の累計
	float m_Time;

	// 陣形安定度
	float m_Stability;
};

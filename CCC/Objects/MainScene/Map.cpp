/**
 * @file   Map.cpp
 *
 * @brief  マップオブジェクトクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/02/04
 *
 * 2026/02/04
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "Map.h"

// 管理クラス
#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/CameraManager.h>

// リソースクラス
#include <CCC/ResourceClasses/ModelResource.h>

// コンポネートクラス
#include <CCC/Components/Transform.h>



// ---------------------------------------------------------------------- //
// パブリック関数
// ---------------------------------------------------------------------- //

Map::Map() :
	ObjectBase()
{
	this->AddComponent<CCC::Components::Transform>("Transform", this);
}

Map::~Map() = default;

void Map::Initialize()
{
}

void Map::Update(float)
{
}

void Map::Render()
{
	CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();	// リソースマネージャーの取得
	CCC::Managers::CameraManager*   p_cm = CCC::Managers::CameraManager  ::GetInstance();	// カメラ管理クラスの取得

	// ワールド行列の設定
	DirectX::SimpleMath::Matrix world = GetComponent<CCC::Components::Transform>("Transform")->GetWorldMatrix();

	// マップのモデルを描画
	p_rm->GetSpriteBatch()->Begin();
	p_rm->GetResource<CCC::Resources::ModelResource>("Map")->GetModel()->Draw(p_rm->GetD3DDeviceContext(), *p_rm->GetCommonStates(), world, p_cm->GetView(), p_rm->GetProjection());
	p_rm->GetSpriteBatch()->End();
}

void Map::Finalize()
{
}

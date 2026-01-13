#include "pch.h"
#include "Floor.h"

#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/CameraManager.h>
#include <CCC/ResourceClasses/ModelResource.h>

#include <CCC/Components/Transform.h>

Floor::Floor()
{
	AddComponent<CCC::Components::Transform>("Transform", this);
}

Floor::~Floor()
{
}

void Floor::Initialize()
{
	GetComponent<CCC::Components::Transform>("Transform")->SetScale(10.0f);
}

void Floor::Update(float elapsedTime)
{
	elapsedTime;
}

void Floor::Render()
{
	// ワールド行列の設定
	DirectX::SimpleMath::Matrix world = GetComponent<CCC::Components::Transform>("Transform")->GetWorldMatrix();

	// リソースマネージャーの取得
	CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();

	// カメラ管理クラスの取得
	CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();

	
	// ゆかのモデルの描画
	p_rm->GetSpriteBatch()->Begin();
	p_rm->GetResource<CCC::Resources::ModelResource>("Floor")->GetModel()->Draw(p_rm->GetD3DDeviceContext(), *p_rm->GetCommonStates(), world, p_cm->GetView(), p_rm->GetProjection());
	p_rm->GetSpriteBatch()->End();
}

void Floor::Finalize()
{
}

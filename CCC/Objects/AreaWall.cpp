#include "pch.h"
#include "AreaWall.h"

#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/CameraManager.h>
#include <CCC/ResourceClasses/ModelResource.h>

#include <CCC/Components/Transform.h>
#include <CCC/Components/Colliders/BoxCollider.h>

AreaWall::AreaWall(float x, float z, float width, float height) :
	ObjectBase(),
	mp_Transform( nullptr )
{
	mp_Transform = this->AddComponent<CCC::Components::Transform>("Transform", this);
	this->AddComponent<CCC::Components::BoxCollider>("Collider", this, mp_Transform, true, width, height);
	
	mp_Transform->SetPositionX(x);
	mp_Transform->SetPositionZ(z);
	mp_Transform->SetScaleX(width / 2.0f);
	mp_Transform->SetScaleZ(height / 2.0f);
}

AreaWall::~AreaWall() = default;

void AreaWall::Initialize()
{
}

void AreaWall::Update(float)
{
}

void AreaWall::Render()
{
	// コライダーのデバッグ表示
#if defined(_DEBUG)
	// ワールド行列の設定
	DirectX::SimpleMath::Matrix world = GetComponent<CCC::Components::Transform>("Transform")->GetWorldMatrix();

	// リソースマネージャーの取得
	CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();

	// カメラ管理クラスの取得
	CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();


	// 天球のモデルの描画
	p_rm->GetSpriteBatch()->Begin();
	p_rm->GetResource<CCC::Resources::ModelResource>("BoxCollider")->GetModel()->Draw(p_rm->GetD3DDeviceContext(), *p_rm->GetCommonStates(), world, p_cm->GetView(), p_rm->GetProjection());
	p_rm->GetSpriteBatch()->End();
#endif
}

void AreaWall::Finalize()
{
}

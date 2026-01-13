#include "pch.h"
#include "SkyDome.h"

#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/CameraManager.h>
#include <CCC/ResourceClasses/ModelResource.h>

#include <CCC/Components/Transform.h>

SkyDome::SkyDome() :
	mp_TargetTransform(nullptr)
{
	AddComponent<CCC::Components::Transform>("Transform", this);
}

SkyDome::~SkyDome() = default;

void SkyDome::Initialize()
{
	GetComponent<CCC::Components::Transform>("Transform")->SetScale(0.5f);
}

void SkyDome::Update(float elapsedTime)
{
	// 警告回避
	elapsedTime;

	// ターゲットの座標を自身の座標に適用
	this->GetComponent<CCC::Components::Transform>("Transform")->SetPosition(mp_TargetTransform->GetPosition());
}

void SkyDome::Render()
{
	// ワールド行列の設定
	DirectX::SimpleMath::Matrix world = GetComponent<CCC::Components::Transform>("Transform")->GetWorldMatrix();

	// リソースマネージャーの取得
	CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();

	// カメラ管理クラスの取得
	CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();


	// 天球のモデルの描画
	p_rm->GetSpriteBatch()->Begin();
	p_rm->GetResource<CCC::Resources::ModelResource>("SkyDome")->GetModel()->Draw(p_rm->GetD3DDeviceContext(), *p_rm->GetCommonStates(), world, p_cm->GetView(), p_rm->GetProjection());
	p_rm->GetSpriteBatch()->End();
}

void SkyDome::Finalize()
{
}

void SkyDome::SetTarget(ObjectBase* p_Target)
{
	mp_TargetTransform = p_Target->GetComponent<CCC::Components::Transform>("Transform");
}

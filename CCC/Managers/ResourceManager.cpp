/**
 * @file   ResourceManaager.cpp
 *
 * @brief  リソース管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/19
 * 
 * 使用を行うには必ずSetResource関数を行う必要がある。
 * 
 * 2026/01/05
 * STABILITY-UIのリソース
 * バー、フレーム、ラベルの追加
 * 
 * 2026/01/06
 * StabilityUIのリソース
 * ワーニング、ブロークンの追加
 * デスの追加
 * 天球の追加
 * 
 * 2026/01/08
 * SkillGaugeUIのリソース追加
 * 
 * 2026/01/09
 * シェーダリソースの追加
 * 
 * 2026/01/13
 * UI関連のシェーダリソースを追加
 * 
 * 2026/01/17
 * Paladinモデル用のアニメーションの追加
 * ・ぶっ飛んで死ぬアニメーション
 * ・起き上がるアニメーション
 * 
 * 2026/01/18
 * 標準ピクセルシェーダの追加
 * 
 * タイトルシーン用のテクスチャを追加
 * 
 * 2026/01/19
 * リザルトシーン用のテクスチャを追加
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "ResourceManager.h"

// Gameクラスの前方宣言
#include <Main/Game.h>

 // リソースクラス
#include <CCC/ResourceClasses/ModelResource.h>
#include <CCC/ResourceClasses/TextureResource.h>
#include <CCC/ResourceClasses/ModelWithBoneResource.h>
#include <CCC/ResourceClasses/AnimationResource.h>
#include <CCC/ResourceClasses/VertexShaderResource.h>
#include <CCC/ResourceClasses/GeometryShaderResource.h>
#include <CCC/ResourceClasses/PixelShaderResource.h>

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //
	void ResourceManager::MakeResources()
	{
		// ---------------------------------------------------------------------- //
		// リソースは必ずここで作成しなければならない
		// ---------------------------------------------------------------------- //

		// ---------------------------------------------------------------------- //
		// すべてのシーンで使用するリソース
		// ---------------------------------------------------------------------- //
		
		// 天球のリソース
		CreateResource<CCC::Resources::ModelResource>("SkyDome", L"Resources/Models/SkyDome/SkyDome.sdkmesh");

		// シェーダ
		CreateResource<CCC::Resources::PixelShaderResource   >("DefaultPS", L"Resources/Shaders/DefaultPS.cso");
		CreateResource<CCC::Resources::VertexShaderResource  >("DefaultVS", L"Resources/Shaders/DefaultVS.cso");
		CreateResource<CCC::Resources::GeometryShaderResource>("DisplayGS", L"Resources/Shaders/DisplayGS.cso");
		CreateResource<CCC::Resources::PixelShaderResource   >("GaugePS"  , L"Resources/Shaders/GaugePS.cso"  );
		CreateResource<CCC::Resources::PixelShaderResource   >("WarningPS", L"Resources/Shaders/WarningPS.cso");




		// ---------------------------------------------------------------------- //
		// タイトルシーンで使用するリソース
		// ---------------------------------------------------------------------- //
		
		// UI
		CreateResource<CCC::Resources::TextureResource>("NewGame" , L"Resources/Textures/NewGameButtom.png" );
		CreateResource<CCC::Resources::TextureResource>("QuitGame", L"Resources/Textures/QuitGameButtom.png");
		CreateResource<CCC::Resources::TextureResource>("Setting" , L"Resources/Textures/SettingButtom.png" );


		// ---------------------------------------------------------------------- //
		// メインシーンで使用するリソース
		// ---------------------------------------------------------------------- //
		
		// パラディンのリソース
		CreateResource<CCC::Resources::ModelWithBoneResource>("Paladin"             , L"Resources/Models/Paladin/Paladin.sdkmesh"                         , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Idle"            , L"Resources/Models/Paladin/Animations/Idle.sdkmesh_anim"            , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Walk"            , L"Resources/Models/Paladin/Animations/Walk.sdkmesh_anim"            , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Walk_Left"       , L"Resources/Models/Paladin/Animations/SideWalk_Left.sdkmesh_anim"   , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Walk_Right"      , L"Resources/Models/Paladin/Animations/SideWalk_Right.sdkmesh_anim"  , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Walk_Back"       , L"Resources/Models/Paladin/Animations/BackWalk.sdkmesh_anim"        , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Run"             , L"Resources/Models/Paladin/Animations/Run.sdkmesh_anim"             , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Slash"           , L"Resources/Models/Paladin/Animations/Slash.sdkmesh_anim"           , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_Death"           , L"Resources/Models/Paladin/Animations/Death.sdkmesh_anim"           , "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_FallingBackDeath", L"Resources/Models/Paladin/Animations/FallingBackDeath.sdkmesh_anim", "MainScene");
		CreateResource<CCC::Resources::AnimationResource>("Paladin_StandUp"         , L"Resources/Models/Paladin/Animations/StandUp.sdkmesh_anim"         , "MainScene");

		// ゆかのリソース
		CreateResource<CCC::Resources::ModelResource>("Floor", L"Resources/Models/Floor/Floor.sdkmesh", "MainScene");

		// StabilityUIのリソース
		CreateResource<CCC::Resources::TextureResource>("Stability_Label"    , L"Resources/Textures/StabilityUI/Label.png"  , "MainScene");
		CreateResource<CCC::Resources::TextureResource>("Stability_Bar"      , L"Resources/Textures/StabilityUI/Bar.png"    , "MainScene");
		CreateResource<CCC::Resources::TextureResource>("Stability_Frame"    , L"Resources/Textures/StabilityUI/Frame.png"  , "MainScene");
		CreateResource<CCC::Resources::TextureResource>("Stability_Warning"  , L"Resources/Textures/StabilityUI/Warning.png", "MainScene");
		CreateResource<CCC::Resources::TextureResource>("Stability_Broken"   , L"Resources/Textures/StabilityUI/Broken.png" , "MainScene");
		CreateResource<CCC::Resources::TextureResource>("Stability_Death"    , L"Resources/Textures/StabilityUI/Death.png"  , "MainScene");

		// SkillGaugeUIのリソース
		CreateResource<CCC::Resources::TextureResource>("SkillGauge_Label_Gladius", L"Resources/Textures/SkillGaugeUI/Label_Gladius.png", "MainScene");
		CreateResource<CCC::Resources::TextureResource>("SkillGauge_Bar"          , L"Resources/Textures/SkillGaugeUI/Bar.png"          , "MainScene");
		CreateResource<CCC::Resources::TextureResource>("SkillGauge_Frame"        , L"Resources/Textures/SkillGaugeUI/Frame.png"        , "MainScene");
		CreateResource<CCC::Resources::TextureResource>("SkillGauge_Bottom_Q"     , L"Resources/Textures/SkillGaugeUI/Bottom_Q.png"     , "MainScene");



		// ---------------------------------------------------------------------- //
		// リザルトシーンで使用するリソース
		// ---------------------------------------------------------------------- //

		// UI
		CreateResource<CCC::Resources::TextureResource>("ResultSceneUI_BackToTitle", L"Resources/Textures/ResultScene/BackToTitle.png", "ResultScene");
		CreateResource<CCC::Resources::TextureResource>("ResultSceneUI_QuitGame"   , L"Resources/Textures/ResultScene/QuitGame.png"   , "ResultScene");
		CreateResource<CCC::Resources::TextureResource>("ResultSceneUI_Win"        , L"Resources/Textures/ResultScene/Win.png"        , "ResultScene");
		CreateResource<CCC::Resources::TextureResource>("ResultSceneUI_Lose"       , L"Resources/Textures/ResultScene/Lose.png"       , "ResultScene");
	}



	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<ResourceManager> ResourceManager::s_Instance = nullptr;

	ResourceManager::ResourceManager() :
		mp_DeviceResources( nullptr ),
		mp_Timer          ( nullptr )
	{
	}

	ResourceManager* ResourceManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new ResourceManager());

		// 実体へのポインタを返す
		return s_Instance.get();
	}


	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	
	void ResourceManager::SetResources(DX::DeviceResources* p_DR, DX::StepTimer* p_Timer)
	{
		// 引数を当てはまるメンバ変数に設定
		mp_DeviceResources = p_DR;
		mp_Timer           = p_Timer;

		//	共通ステートオブジェクトの作成
		m_CommonStates = std::make_unique<DirectX::CommonStates>(mp_DeviceResources->GetD3DDevice());


		// 射影行列の作成
		RECT rect = mp_DeviceResources->GetOutputSize();
		m_Proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			DirectX::XMConvertToRadians(45.0f),
			static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
			0.1f, 2000.0f
		);

		// スプライトバッチの作成
		m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(mp_DeviceResources->GetD3DDeviceContext());

		// リソースの生成
		MakeResources();

		// リソースの読み込み
		LoadResources();
	}

	void ResourceManager::LoadResources(const std::string& scene)
	{
		for (const auto& [name, resouce] : m_Resources)
			if (resouce->GetUseScene() == scene)
				resouce->Load();
	}

	void ResourceManager::DeleteResources(const std::string& scene)
	{
		for (const auto& [name, resouce] : m_Resources)
			if (resouce->GetUseScene() == scene)
				resouce->Unload();
	}

	ResourceManager::~ResourceManager()
	{
		// リソースの破棄
		DeleteResources();
	}
}

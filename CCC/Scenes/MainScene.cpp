/**
 * @file   MainScene.cpp
 *
 * @brief  メインシーンのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/06
 * 
 * 2025/12/19
 * ポーンオブジェクトクラスの追加
 * 
 * 2026/01/05
 * 陣形密度の表示
 * 
 * 2026/01/06
 * 陣形崩壊後の安定するまでの回復時間をデバッグ表示
 */

#include "pch.h"
#include "MainScene.h"

#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/CameraManager.h>
#include <CCC/Managers/ObjectManager.h>
#include <CCC/Managers/InputManager.h>
#include <CCC/Managers/SceneManager.h>

#include <VertexTypes.h> 

#include <CCC/Components/Camera.h>

#include <CCC/Objects/SkyDome.h>

#include <CCC/Objects/MainScene/MainCamera/MainCamera.h>
#include <CCC/Objects/MainScene/Floor.h>
#include <CCC/Objects/MainScene/PawnLeader/PawnLeader.h>
#include <CCC/Objects/MainScene/UI/StabilityUI.h>
#include <CCC/Objects/MainScene/UI/SkillGaugeUI.h>

MainScene::MainScene() :
	CCC::Bases::SceneBase(),
	mp_ResourceManager( CCC::Managers::ResourceManager::GetInstance() ),
	mp_InputManager   ( CCC::Managers::InputManager   ::GetInstance() ),
	mp_SceneManager   ( CCC::Managers::SceneManager   ::GetInstance() ),
	m_CameraMode      ( CameraMode::Main ),
	mp_PawnLeader     ( nullptr )
{
}

MainScene::~MainScene() = default;

void MainScene::Initialize()
{
	using namespace DirectX;

	// デバッグカメラの作成
	RECT rect = mp_ResourceManager->GetDeviceResources()->GetOutputSize();
	m_DebugCamera = std::make_unique<CCC::Debug::DebugCamera>(rect.right, rect.bottom);



	//	エフェクトの作成 
	m_BatchEffect = std::make_unique<AlphaTestEffect>(mp_ResourceManager->GetD3DDevice());
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	//	入力レイアウト生成 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	mp_ResourceManager->GetD3DDevice()->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	// カメラモードを初期化
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);




	// ---------------------------------------------------------------------- //
	// オブジェクトの生成
	// ---------------------------------------------------------------------- //

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	// 床の生成
	p_om->CreateObject<Floor>("Floor");

	// リーダーポーンの生成
	mp_PawnLeader = p_om->CreateObject<PawnLeader>("PawnLeader");
	mp_PawnLeader->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 5.0f));
	
	// カメラの生成
	p_om->CreateObject<MainCamera>("MainCamera")->SetTarget(mp_PawnLeader);

	// 天球の生成
	p_om->CreateObject<SkyDome>("SkyDome")->SetTarget(mp_PawnLeader);

	// UIの生成
	p_om->CreateObject<StabilityUI >("StabilityUI" )->SetTarget(mp_PawnLeader);
	p_om->CreateObject<SkillGaugeUI>("SkillGaugeUI")->SetTarget(mp_PawnLeader);



	// ---------------------------------------------------------------------- //
	// デバッグ機能の生成
	// ---------------------------------------------------------------------- //

	// 生成に必要なポインタの取得
	ID3D11Device1* device = mp_ResourceManager->GetD3DDevice();
	ID3D11DeviceContext1* context = mp_ResourceManager->GetD3DDeviceContext();
	
	//	デバッグフォントの生成
	m_DebugFont = std::make_unique<CCC::Debug::DebugFont>(
			device, context,
			L"Resources\\Font\\SegoeUI_18.spritefont"
		);
}

void MainScene::Update(float elapsedTime)
{
	elapsedTime;

	// デバッグカメラの更新
	m_DebugCamera->Update();

	// カメラモードの切り替え
	if (DirectX::Keyboard::Get().GetState().N)
	{
		m_CameraMode = CameraMode::Debug;
		DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
	if (DirectX::Keyboard::Get().GetState().M)
	{
		m_CameraMode = CameraMode::Main;
		DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);
	}

	// シーンの切り替え
	if (DirectX::Keyboard::Get().GetState().I)
	{
		ChangeScene("SampleSceneSec");
		DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
}

void MainScene::Render()
{
	using namespace DirectX;

	// カメラ管理クラスの取得
	CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	switch (m_CameraMode)
	{
	case MainScene::CameraMode::Debug:
		p_cm->SetView(m_DebugCamera->GetCameraMatrix());
		break;
	case MainScene::CameraMode::Main:
		p_cm->SetView(p_om->GetObject<CCC::Bases::ObjectBase>("MainCamera")->GetComponent<CCC::Components::Camera>("Camera")->GetView());
		break;
	default:
		p_cm->SetView(m_DebugCamera->GetCameraMatrix());
		break;
	}


	// ---------------------------------------------------------------------- //
	// デバッグ情報の描画
	// ---------------------------------------------------------------------- //

#if defined(_DEBUG)
	//	FPSを取得する
	uint32_t fps = mp_ResourceManager->GetStepTimer()->GetFramesPerSecond();

	//	FPSの表示
	m_DebugFont->AddString(0, 0, DirectX::Colors::White, L"FPS=%d", fps);


	// 現在のシーン名の表示
	m_DebugFont->AddString(
		mp_SceneManager->GetCurrentSceneNameUTF16().c_str(),
		DirectX::SimpleMath::Vector2(0, 50),
		DirectX::Colors::White
	);


	// マウスの状態を取得
	int mousePosX = mp_InputManager->GetInputAs<int>("CameraHorizontal");
	int mousePosY = mp_InputManager->GetInputAs<int>("CameraVertical");

	// マウスの座標を表示
	m_DebugFont->AddString(0, 100, DirectX::Colors::White, L"Mouse Position=(%d, %d)", mousePosX, mousePosY);


	
	float formationAvarage = mp_PawnLeader->GetAverageUnitDiff();				// 陣形所定位置との差分の平均
	float fomationStability          = mp_PawnLeader->GetFormationStability();	// 陣形安定度の取得
	std::wstring formationEvaluation = L"WARNING";								// 安定度の評価

	// 安定度から評価する
	if      (fomationStability > 0.7f ) formationEvaluation = L"STABLE";
	else if (fomationStability < 0.01f) formationEvaluation = L"BROKEN";

	// 陣形安定度を表示
	m_DebugFont->AddString(0, 200, DirectX::Colors::White, L"Formation = %.2f\n%s\nAvarage = %.2f", fomationStability, formationEvaluation.c_str(), formationAvarage);



	float recoveryTime = mp_PawnLeader->GetRecoveryTime();	// 回復までの時間
	m_DebugFont->AddString(0, 300, DirectX::Colors::White, L"RecoveryTime = %.0f", recoveryTime);



	// 状態をデバッグ表示
	PawnLeader::SkillStates SkillState = mp_PawnLeader->GetSkillState();
	float skillGauge = mp_PawnLeader->GetSkillGauge();
	std::wstring skillStateEvaluation;
	switch (SkillState)
	{
	case PawnLeader::SkillStates::Inactive:
		skillStateEvaluation = L"Inactive";
		break;
	case PawnLeader::SkillStates::Active:
		skillStateEvaluation = L"Active";
		break;
	case PawnLeader::SkillStates::Recovering:
		skillStateEvaluation = L"Recovering";
		break;
	case PawnLeader::SkillStates::Count:
	default:
		skillStateEvaluation = L"Error";
		break;
	}
	m_DebugFont->AddString(0, 350, DirectX::Colors::White, L"SkillState : %s\nSkillGauge : %.2f", skillStateEvaluation.c_str(), skillGauge);

	//	デバッグフォントの描画
	m_DebugFont->Render(mp_ResourceManager->GetCommonStates());

#endif
}

void MainScene::Finalize()
{
}

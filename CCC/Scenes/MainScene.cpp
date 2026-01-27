/**
 * @file   MainScene.cpp
 *
 * @brief  メインシーンのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/28
 * 
 * 2025/12/19
 * 作成
 * ポーンオブジェクトクラスの追加
 * 
 * 2026/01/05
 * 陣形密度の表示
 * 
 * 2026/01/06
 * 陣形崩壊後の安定するまでの回復時間をデバッグ表示
 * 
 * 2026/01/18
 * コメントを追加
 * 
 * 2026/01/22
 * BTによる状態の変化をデバッグ表示
 * 
 * 2026/01/23
 * 敵オブジェクトを複製
 * 
 * 2026/01/28
 * BGM戦闘01の追加
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "MainScene.h"


// DirectXTK
#include <VertexTypes.h> 


// 管理クラス
#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/CameraManager.h>
#include <CCC/Managers/ObjectManager.h>
#include <CCC/Managers/InputManager.h>
#include <CCC/Managers/SceneManager.h>
#include <CCC/Managers/GameContextManager.h>


// メッセンジャー
#include <CCC/Messenger/MessengerHub.h>
#include <CCC/Messenger/MessageType.h>

// コンポネート
#include <CCC/Components/Camera.h>


// このシーンで使用するオブジェクト
#include <CCC/Objects/SkyDome.h>
#include <CCC/Objects/PawnManager.h>
#include <CCC/Objects/MainScene/MainCamera/MainCamera.h>
#include <CCC/Objects/MainScene/Floor.h>
#include <CCC/Objects/MainScene/PawnLeader/PawnLeader.h>
#include <CCC/Objects/MainScene/EnemyPawnLeader/EnemyPawnLeader.h>
#include <CCC/Objects/MainScene/EnemyPawnLeader/EnemyPawnLeaderBT.h>
#include <CCC/Objects/MainScene/UI/StabilityUI.h>
#include <CCC/Objects/MainScene/UI/SkillGaugeUI.h>



// ---------------------------------------------------------------------- //
// パブリック関数
// ---------------------------------------------------------------------- //

MainScene::MainScene() :
	CCC::Bases::SceneBase(),
	mp_ResourceManager( CCC::Managers::ResourceManager::GetInstance() ),
	mp_InputManager   ( CCC::Managers::InputManager   ::GetInstance() ),
	mp_SceneManager   ( CCC::Managers::SceneManager   ::GetInstance() ),
	m_CameraMode      ( CameraMode::Main ),
	mp_PawnLeader     ( nullptr ),
	mp_EnemyFirst     ( nullptr ),
	mp_EnemySecond    ( nullptr )
{
}

MainScene::~MainScene() = default;

void MainScene::Initialize()
{
	// ---------------------------------------------------------------------- //
	// 初期化処理
	// ---------------------------------------------------------------------- //

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
	Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);

	// メッセンジャーハブの取得
	CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();

	// BGMの戦闘01を再生
	p_mh->Receive(CCC::Messenger::MessageType::BGM_PLAY_BATTLE_01, CCC::Messenger::MessengerHub::PayLoad());




	// ---------------------------------------------------------------------- //
	// メッセンジャーに登録
	// ---------------------------------------------------------------------- //
	
	// リザルトシーンに遷移することを要求するメッセージ
	p_mh->Subscribe(CCC::Messenger::MessageType::Request_ResultScene,
		[](const CCC::Messenger::MessengerHub::PayLoad& is)
		{
			if (const bool* p = std::any_cast<bool>(&is.item))
			{
				CCC::Managers::SceneManager* p_sm = CCC::Managers::SceneManager::GetInstance();
				p_sm->RequestSceneChange("ResultScene");
			}
		});

	// 敵の隊長Pawnの状態変化があったらデバッグ表示用として保存する処理
	p_mh->Subscribe(CCC::Messenger::MessageType::RequestToEnemyPawnLeader_State,
		[this](const CCC::Messenger::MessengerHub::PayLoad& is)
		{
			if (const AddressedPayload* p = std::any_cast<AddressedPayload>(&is.item))
			{
				switch (p->p_Payload)
				{
				case EnemyPawnLeaderStates::Chase:
					m_BT = L"Chase";
					break;
				case EnemyPawnLeaderStates::Attack:
					m_BT = L"Attack";
					break;
				case EnemyPawnLeaderStates::Wait:
				default:
					m_BT = L"Wait";
					break;
				}
			}
		});



	// ---------------------------------------------------------------------- //
	// オブジェクトの生成
	// ---------------------------------------------------------------------- //

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	// 床の生成
	p_om->CreateObject<Floor>("Floor");

	// ポーンマネージャの生成
	PawnManager* p_PawnManager = p_om->CreateObject<PawnManager>("PawnManager");

	// リーダーポーンの生成
	mp_PawnLeader = p_om->CreateObject<PawnLeader>("PawnLeader", p_PawnManager);
	mp_PawnLeader->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 5.0f));

	// 敵リーダーポーン１の生成
	mp_EnemyFirst = p_om->CreateObject<EnemyPawnLeader>(
		"EnemyFirst",
		DirectX::SimpleMath::Vector3(20.0f, 0.0f, -10.0f),
		p_PawnManager,
		mp_PawnLeader,
		"First");
	// BTの生成
	p_om->CreateObject<EnemyPawnLeaderBT>("EnemyFirstBT", mp_EnemyFirst);

	// 敵リーダーポーン２の生成
	mp_EnemySecond = p_om->CreateObject<EnemyPawnLeader>(
		"EnemySecond",
		DirectX::SimpleMath::Vector3(-20.0f, 0.0f, 30.0f),
		p_PawnManager,
		mp_PawnLeader,
		"Second");
	// BTの生成
	p_om->CreateObject<EnemyPawnLeaderBT>("EnemySecondBT", mp_EnemySecond);
	
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

	// 敵の隊長Pawnの状態のデバッグ表示
	m_BT = L"";
}

void MainScene::Update(float)
{
	// 勝利判定
	if (mp_EnemyFirst->GetStabilityState() == EnemyPawnLeader::StabilityStates::Death &&
		mp_EnemySecond->GetStabilityState() == EnemyPawnLeader::StabilityStates::Death)
	{
		// ゲームデータに「勝利」を送る
		CCC::Managers::GameContextManager::GetInstance()->
			SetGameData("PlayerIsWin", true);

		// リザルトシーンに遷移することを要求
		CCC::Messenger::MessengerHub::GetInstance()->
			Receive(
				CCC::Messenger::MessageType::Request_ResultScene,
				CCC::Messenger::MessengerHub::PayLoad(true)
			);
	}


	// ---------------------------------------------------------------------- //
	// デバッグ関連の更新
	// ---------------------------------------------------------------------- //

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
}

void MainScene::Render()
{
	// ---------------------------------------------------------------------- //
	// 描画処理
	// ---------------------------------------------------------------------- //

	using namespace DirectX;

	// カメラ管理クラスの取得
	CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	// ビュー行列の取得
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


	// 攻撃状態かをデバッグ憑依
	bool isAttack = mp_PawnLeader->IsAttacking();
	m_DebugFont->AddString(0, 420, DirectX::Colors::White, L"IsAttack : %s", isAttack ? L"TRUE" : L"FALSE");

	// BTの判断をデバッグ表示
	m_DebugFont->AddString(0, 500, DirectX::Colors::White, L"Behavior Tree : %s", m_BT.c_str());

	//	デバッグフォントの描画
	m_DebugFont->Render(mp_ResourceManager->GetCommonStates());

#endif
}

void MainScene::Finalize()
{
	// メッセンジャーハブの取得
	CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();

	// BGMの戦闘01を停止
	p_mh->Receive(CCC::Messenger::MessageType::BGM_STOP_BATTLE_01, CCC::Messenger::MessengerHub::PayLoad());

	// 登録したメッセージを削除する
	p_mh->Unsubscribe(CCC::Messenger::MessageType::Request_ResultScene);
	p_mh->Unsubscribe(CCC::Messenger::MessageType::RequestToEnemyPawnLeader_State);
}

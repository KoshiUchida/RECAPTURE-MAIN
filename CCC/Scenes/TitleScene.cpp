/**
 * @file   TitleScene.cpp
 *
 * @brief  タイトルシーンのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/28
 *
 * 2026/01/18
 * 作成
 * 
 * 2026/01/19
 * メッセンジャーの種別名の変に伴い、ゲーム終了ボタンの記述を変更
 * 
 * 2026/01/28
 * スタートボタンを押すとSEがなるように変更
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "TitleScene.h"


// 画面情報
#include <Main/DisplayInfo.h>

// 管理クラス
#include <CCC/Managers/ObjectManager.h>
#include <CCC/Managers/SceneManager.h>

// メッセンジャー
#include <CCC/Messenger/MessengerHub.h>
#include <CCC/Messenger/MessageType.h>

// このシーンで使用するオブジェクト
#include <CCC/Objects//ButtomUIBase.h>
#include <CCC/Objects/TextureRenderer.h>



// ---------------------------------------------------------------------- //
// パブリック関数
// ---------------------------------------------------------------------- //

TitleScene::TitleScene() :
	CCC::Bases::SceneBase()
{
}

TitleScene::~TitleScene() = default;

void TitleScene::Initialize()
{
	// ---------------------------------------------------------------------- //
	// 初期化処理
	// ---------------------------------------------------------------------- //
	
	// マウスモードをABSOLUTEに設定
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	// メッセンジャーハブの取得
	CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();



	// ---------------------------------------------------------------------- //
	// メッセンジャーに登録
	// ---------------------------------------------------------------------- //

	// ボタン用のメッセージを作成
	p_mh->Subscribe(CCC::Messenger::MessageType::Buttom_Start_Released,
		[](const CCC::Messenger::MessengerHub::PayLoad&)
		{
			CCC::Managers::SceneManager* p_sm = CCC::Managers::SceneManager::GetInstance();
			p_sm->RequestSceneChange("MainScene");

			CCC::Messenger::MessengerHub::GetInstance()->Receive(CCC::Messenger::MessageType::SE_PLAY_START_BUTTOM, CCC::Messenger::MessengerHub::PayLoad());
		});
	p_mh->Subscribe(CCC::Messenger::MessageType::Buttom_QuitGame_Released,
		[](const CCC::Messenger::MessengerHub::PayLoad&)
		{
			CCC::Managers::SceneManager* p_sm = CCC::Managers::SceneManager::GetInstance();
			p_sm->End();
		});


	// ---------------------------------------------------------------------- //
	// オブジェクトの生成
	// ---------------------------------------------------------------------- //

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	p_om->CreateObject<CCC::Bases::TextureRenderer>(
		"Logo",
		DirectX::SimpleMath::Vector2(DisplayInfo::Width * 0.5f, DisplayInfo::Height * 0.3f),
		0.5f, "TitleLogo"
	);
	p_om->CreateObject<CCC::Bases::ButtomUIBase>(
		"StartButtom",
		DirectX::SimpleMath::Vector2(DisplayInfo::Width * 0.5f, DisplayInfo::Height * 0.7f),
		0.5f, "NewGame",
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::Buttom_Start_Released
		);

	p_om->CreateObject<CCC::Bases::ButtomUIBase>(
		"EndButtom",
		DirectX::SimpleMath::Vector2(DisplayInfo::Width * 0.5f, DisplayInfo::Height * 0.8f),
		0.5f, "QuitGame",
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::Buttom_QuitGame_Released
	);
}

void TitleScene::Update(float elapsedTime)
{
	// 警告回避
	elapsedTime;
}

void TitleScene::Render()
{
}

void TitleScene::Finalize()
{
	CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();
	p_mh->Unsubscribe(CCC::Messenger::MessageType::Buttom_Start_Released);
	p_mh->Unsubscribe(CCC::Messenger::MessageType::Buttom_QuitGame_Released);
}

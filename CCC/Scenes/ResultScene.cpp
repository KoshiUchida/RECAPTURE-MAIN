/**
 * @file   ResultScene.cpp
 *
 * @brief  リザルトシーンのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/19
 *
 * 2026/01/18
 * 作成
 * 
 * 2026/01/19
 * ボタンの表示するテクスチャの差し換え
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "ResultScene.h"


// 画面情報
#include <Main/DisplayInfo.h>

// 管理クラス
#include <CCC/Managers/ObjectManager.h>
#include <CCC/Managers/SceneManager.h>
#include <CCC/Managers/GameContextManager.h>

// メッセンジャー
#include <CCC/Messenger/MessengerHub.h>
#include <CCC/Messenger/MessageType.h>

// このシーンで使用するオブジェクト
#include <CCC/Objects/ButtomUIBase.h>
#include <CCC/Objects/TextureRenderer.h>



// ---------------------------------------------------------------------- //
// パブリック関数
// ---------------------------------------------------------------------- //

ResultScene::ResultScene() :
	CCC::Bases::SceneBase()
{
}

ResultScene::~ResultScene() = default;

void ResultScene::Initialize()
{
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	// ---------------------------------------------------------------------- //
	// メッセンジャーに登録
	// ---------------------------------------------------------------------- //

	CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();
	p_mh->Subscribe(CCC::Messenger::MessageType::Buttom_BackToTitle_Released,
		[](const CCC::Messenger::MessengerHub::PayLoad& is)
		{
			if (const bool* p = std::any_cast<bool>(&is.item))
			{
				CCC::Managers::SceneManager* p_sm = CCC::Managers::SceneManager::GetInstance();
				p_sm->RequestSceneChange("TitleScene");
			}
		});
	p_mh->Subscribe(CCC::Messenger::MessageType::Buttom_QuitGame_Released,
		[](const CCC::Messenger::MessengerHub::PayLoad& is)
		{
			if (const bool* p = std::any_cast<bool>(&is.item))
			{
				CCC::Managers::SceneManager* p_sm = CCC::Managers::SceneManager::GetInstance();
				p_sm->End();
			}
		});


	// ---------------------------------------------------------------------- //
	// オブジェクトの生成
	// ---------------------------------------------------------------------- //

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();
	
	p_om->CreateObject<CCC::Bases::TextureRenderer>(
		"WinTexture",
		DirectX::SimpleMath::Vector2(DisplayInfo::Width * 0.5f, DisplayInfo::Height * 0.3f),
		0.5f,

		// 勝敗によって表示を変える
		CCC::Managers::GameContextManager::GetInstance()->GetDataAs<bool>("PlayerIsWin") ? "ResultSceneUI_Win" : "ResultSceneUI_Lose"
	);
	
	p_om->CreateObject<CCC::Bases::ButtomUIBase>(
		"BackToTitleButtom",
		DirectX::SimpleMath::Vector2(DisplayInfo::Width * 0.5f, DisplayInfo::Height * 0.75f),
		0.4f, "ResultSceneUI_BackToTitle",
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::Buttom_BackToTitle_Released
	);
	p_om->CreateObject<CCC::Bases::ButtomUIBase>(
		"QuitGameButtom",
		DirectX::SimpleMath::Vector2(DisplayInfo::Width * 0.5f, DisplayInfo::Height * 0.9f),
		0.4f, "ResultSceneUI_QuitGame",
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::NONE,
		CCC::Messenger::MessageType::Buttom_QuitGame_Released
	);
}

void ResultScene::Update(float elapsedTime)
{
	// 警告回避
	elapsedTime;
}

void ResultScene::Render()
{
}

void ResultScene::Finalize()
{
	CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();
	p_mh->Unsubscribe(CCC::Messenger::MessageType::Buttom_BackToTitle_Released);
	p_mh->Unsubscribe(CCC::Messenger::MessageType::Buttom_QuitGame_Released);
}

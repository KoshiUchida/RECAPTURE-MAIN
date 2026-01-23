/**
 * @file   InputManager.cpp
 *
 * @brief  シーンの管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * 
 * 2026/01/08
 * コメント追加
 * 
 * 2026/01/18
 * 「Aボタン」の入力を追加
 * メッセンジャー実装の初期段階としてAボタンを送信する機能を実装
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "InputManager.h"

// メッセンジャー
#include <CCC/Messenger/MessageType.h>
#include <CCC/Messenger/MessengerHub.h>

#include <Main/DisplayInfo.h>

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<InputManager> InputManager::s_Instance = nullptr;

	InputManager::InputManager() :
		mp_Mouse    (nullptr),
		mp_Keyboard (nullptr),
		m_MouseState()
	{
		// 初期化処理
		Initialize();

		// 登録処理
		Registration();
	}

	InputManager* InputManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new InputManager());

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	InputManager::~InputManager() = default;

	void InputManager::Update()
	{
		// マウスの状態を取得する
		m_MouseState = mp_Mouse->GetState();

		// キーボードの状態を取得する
		m_KeyboardState = mp_Keyboard->GetState();


		if (this->GetInputAs<bool>("Attack"))
		{
			CCC::Messenger::MessengerHub::GetInstance()->Receive(
				CCC::Messenger::MessageType::INPUT_ATTACK,
				CCC::Messenger::MessengerHub::PayLoad{true}
			);
		}

		if (this->GetInputAs<bool>("A"))
		{
			CCC::Messenger::MessengerHub::GetInstance()->Receive(
				CCC::Messenger::MessageType::INPUT_A,
				CCC::Messenger::MessengerHub::PayLoad{
					DirectX::SimpleMath::Vector2(static_cast<float>(m_MouseState.x) / DisplayInfo::Width * DisplayInfo::ScreenWidth, static_cast<float>(m_MouseState.y) / DisplayInfo::Height * DisplayInfo::ScreenHeight)
			}
			);
		}
	}



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //
	void InputManager::Initialize()
	{
		// マウスの取得
		mp_Mouse = &DirectX::Mouse::Get();

		// キーボードの取得
		mp_Keyboard = &DirectX::Keyboard::Get();
	}

	void InputManager::Registration()
	{
		// ---------------------------------------------------------------------- //
		// 入力情報の定義はここで行う
		// ---------------------------------------------------------------------- //

		AddInputs("CameraHorizontal", [this]() {
			return m_MouseState.x;
			});

		AddInputs("CameraHorizontalPercent", [this]() {
			return static_cast<float>(m_MouseState.x) / 100.0f;
			});

		AddInputs("CameraVertical", [this]() {
			return m_MouseState.y;
			});

		AddInputs("CameraVerticalPercent", [this]() {
			return static_cast<float>(m_MouseState.y) / 100.0f;
			});


		AddInputs("Horizontal", [this]() {
			int horizontal = 0;

			if (m_KeyboardState.A || m_KeyboardState.Left)
				horizontal -= 1;

			if (m_KeyboardState.D || m_KeyboardState.Right)
				horizontal += 1;

			return horizontal;
			});

		AddInputs("Vertical", [this]() {
			int vertical = 0;

			if (m_KeyboardState.W || m_KeyboardState.Up)
				vertical -= 1;

			if (m_KeyboardState.S || m_KeyboardState.Down)
				vertical += 1;

			return vertical;
			});

		AddInputs("Dash", [this]() {
			return m_KeyboardState.LeftShift;
			});

		AddInputs("Attack", [this]() {
			return m_MouseState.leftButton;
			});

		AddInputs("MoveInput", [this]() {
			return GetInputAs<int>("Horizontal") != 0 || GetInputAs<int>("Vertical") != 0;
			});

		AddInputs("Skill", [this]() {
			return m_KeyboardState.Q;
			});

		AddInputs("A", [this]() {
			return m_MouseState.leftButton;
			});
	}



	// ---------------------------------------------------------------------- //
	// 入力管理関連
	// ---------------------------------------------------------------------- //

	void InputManager::AddInputs(const std::string& name, const std::function<std::any()>& fanction)
	{
		// 関数を登録する
		m_Inputs.emplace(name, fanction);
	}

	std::any InputManager::GetInput(const std::string& name)
	{
		return m_Inputs[name]();
	}
}

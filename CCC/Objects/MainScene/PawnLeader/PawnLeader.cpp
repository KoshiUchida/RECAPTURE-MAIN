/**
 * @file   PawnLeader.cpp
 *
 * @brief  ポーンリーダーオブジェクトクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/19
 * 
 * 2026/01/05
 * 隊員の陣形所定位置平均を求める機能の実装
 * パラメータファイルを作成したため、定数群を移動
 * 
 * 2026/01/06
 * 陣形安定度の評価方法を変更
 * 
 * 2026/01/10
 * 移動していると唐突に止まる問題の解決
 * 
 * 2026/01/18
 * 陣形が死ぬとリザルトシーンに遷移することを要求するように改修
 * 
 * 2026/01/19
 * 陣形が死ぬと「敗北」をゲームデータに入力。
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "PawnLeader.h"

// パラメータ
#include "PawnLeaderParameter.h"

 // 管理クラス
#include <CCC/Managers/InputManager.h>
#include <CCC/Managers/ObjectManager.h>
#include <CCC/Managers/CameraManager.h>
#include <CCC/Managers/GameContextManager.h>

// メッセンジャー
#include <CCC/Messenger/MessengerHub.h>
#include <CCC/Messenger/MessageType.h>

// 関係のあるオブジェクトクラス
#include <CCC/Objects/MainScene/Pawn/Pawn.h>
#include <CCC/Objects/PawnManager.h>

// コンポネート
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

#include <CCC/Objects/TeamID.h>



 // ---------------------------------------------------------------------- //
 // パブリック関数
 // ---------------------------------------------------------------------- //

PawnLeader::PawnLeader(PawnManager* p_PawnManager) :
	PawnBase(TeamID::Player),
	mp_InputManager (CCC::Managers::InputManager::GetInstance()),
	mp_CameraManager(CCC::Managers::CameraManager::GetInstance()),
	mp_CameraTransform(nullptr),
	mp_PawnManager(p_PawnManager),
	m_AverageUnitDiff(0.0f),
	m_StabilityState(StabilityStates::Stable),
	m_IsBroken(false),
	m_BrokenTime(0.0f),
	m_RecoveryTime(0.0f),
	m_SkillState(SkillStates::Recovering),
	m_SkillGauge(0.0f),
	m_IsMove(false)
{
	this->AddComponent<CCC::Components::PawnCollider>("Collider", this, 1.0f, p_PawnManager);
}

PawnLeader::~PawnLeader()
{
	//if (CCC::Messenger::MessengerHub* messenger = CCC::Messenger::MessengerHub::GetInstance())
	//	messenger->Unsubscribe(CCC::Messenger::MessageType::INPUT_ATTACK);
}

void PawnLeader::Initialize()
{
	// モデルステートにアニメーションプレイヤーを追加
	AddAnimationPlayer("Paladin_Idle");
	AddAnimationPlayer("Paladin_Walk");
	AddAnimationPlayer("Paladin_Run");
	AddAnimationPlayer("Paladin_Slash");

	// サイズを0.02に設定
	SetScale(0.02);



	// ---------------------------------------------------------------------- //
	// 部隊の生成
	// ---------------------------------------------------------------------- //
	
	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	for (int i = 0; i < PawnLeaderParameter::NUMBER_PAWN; i++)
	{
		// ポーンの生成
		Pawn* p_Pawn = p_om->CreateObject<Pawn>("Pawn" + std::to_string(i), TeamID::Player, mp_PawnManager);
		p_Pawn->SetTarget(this);

		// ポーンポインタ配列に追加
		m_PawnPointers.push_back(p_Pawn);
	}

	// 陣形の設定
	FormationSquare(PawnLeaderParameter::SPACING_BETWEEN_PAWN);

	// 位置リセット
	PawnsPositionReset();



	// ---------------------------------------------------------------------- //
	// メッセージの作成
	// ---------------------------------------------------------------------- //

	CCC::Messenger::MessengerHub* messenger = CCC::Messenger::MessengerHub::GetInstance();


	messenger->Subscribe(CCC::Messenger::MessageType::INPUT_ATTACK,
		[this](const CCC::Messenger::MessengerHub::PayLoad& is) {
			if (auto p = std::any_cast<bool>(&is.item))
			{
				if (*p) this->SetIsAttack(true);
			}
		}
	);
}

void PawnLeader::Process(float elapsedTime)
{
	// ---------------------------------------------------------------------- //
	// 攻撃処理
	// ---------------------------------------------------------------------- //

	if (this->IsAttacking())
	{
		if (this->EndCurrentAnimation())
			this->SetIsAttack(false);
	}
	//else if (mp_InputManager->GetInputAs<bool>("Attack"))
	//{
	//	this->SetIsAttack(true);
	//}

	// ---------------------------------------------------------------------- //
	// 陣形スキル
	// ---------------------------------------------------------------------- //
	switch (m_SkillState)
	{
	case PawnLeader::SkillStates::Inactive:

		if (!this->IsAttacking() && mp_InputManager->GetInputAs<bool>("Skill") && m_StabilityState == StabilityStates::Stable)
		{
			m_SkillState = SkillStates::Active;
			FormationWedge(PawnLeaderParameter::SPACING_BETWEEN_PAWN);
			this->SetIsSkillActive(true);


			float x = this->GetTransform()->GetRotateX();
			m_SkillAngle = DirectX::SimpleMath::Vector3(-std::sinf(x), 0.0f, -std::cosf(x));
		}

		break;
	case PawnLeader::SkillStates::Active:

		m_SkillGauge -= PawnLeaderParameter::SkillGauge::CONSUMPTION_RATE * elapsedTime;

		if (m_SkillGauge <= 0.0f || m_StabilityState == StabilityStates::Broken)
		{
			m_SkillGauge = 0.0f;
			m_SkillState = SkillStates::Recovering;
			FormationSquare(PawnLeaderParameter::SPACING_BETWEEN_PAWN);
			this->SetIsSkillActive(false);
		}

		break;
	case PawnLeader::SkillStates::Recovering:

		m_SkillGauge += PawnLeaderParameter::SkillGauge::RECOVERY_RATE * this->GetFormationStability() * elapsedTime;

		if (m_SkillGauge >= 1.0f)
		{
			m_SkillGauge = 1.0f;
			m_SkillState = SkillStates::Inactive;
		}

		break;
	case PawnLeader::SkillStates::Count:
	default:
		// 通常ならありえない処理
		m_SkillState = PawnLeader::SkillStates::Inactive;
		break;
	}


	// ---------------------------------------------------------------------- //
	// 移動機能
	// ---------------------------------------------------------------------- //

	if (!this->IsAttacking())
	{

		// もし、操作が入ったら現在の回転からベロシティを設定する
		bool moveInput = mp_InputManager->GetInputAs<bool>("MoveInput");
		if (!m_IsMove && moveInput)
		{
			float rotationX = this->GetTransform()->GetRotateX();
			SetVelocity(DirectX::SimpleMath::Vector3(-std::sinf(rotationX), 0.0f, -std::cosf(rotationX)));
			m_IsMove = true;
		}
		else if (m_IsMove && !moveInput)
		{
			m_IsMove = false;
		}


		// 入力方向の取得
		const float horizontal = DirectX::XM_PI * static_cast<float>(mp_InputManager->GetInputAs<int>("Horizontal"));
		const float vertical = DirectX::XM_PI * static_cast<float>(mp_InputManager->GetInputAs<int>("Vertical"));

		float forwardInput = -static_cast<float>(vertical);
		float rightInput = static_cast<float>(horizontal);

		// もし、スキル発動中なら自動で前進する
		if (this->IsSkillActive())
		{
			forwardInput = 1.0f;
			rightInput   = 0.0f;
		}


		// カメラ基準ベクトル取得
		const DirectX::SimpleMath::Vector3 forward = mp_CameraManager->GetForwardXZ();
		const DirectX::SimpleMath::Vector3 right = mp_CameraManager->GetRightXZ();

		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Zero;
		if (this->IsSkillActive())
			direction = m_SkillAngle * forwardInput;
		else
			direction = forward * forwardInput;

		// もし、スキル発動中または陣形安定度が崩壊しているのだったら旋回速度にデバフ
		if (this->IsSkillActive() || m_StabilityState == StabilityStates::Broken)
			direction += right * rightInput * PawnLeaderParameter::SkillGauge::ROTATION_DEBUFF;
		else
			direction += right * rightInput;

		direction.Normalize();

		// 目標ベロシティ
		DirectX::SimpleMath::Vector3 DesiredVelocity = direction;

		// 速度の設定
		if (this->IsSkillActive() || mp_InputManager->GetInputAs<bool>("Dash"))
			DesiredVelocity *= PawnLeaderParameter::RUN_SPEED;
		else
			DesiredVelocity *= PawnLeaderParameter::MOVE_SPEED;

		// もし、仕切る発動中なら速度にバフ
		if (this->IsSkillActive())
			DesiredVelocity *= PawnLeaderParameter::SkillGauge::SPEED_BUFF;

		// ベロシティの変更
		if ((DesiredVelocity - GetVelocity()).Length() > PawnLeaderParameter::STOP_RADIUS)
			AddVelocity((DesiredVelocity - GetVelocity()) * PawnLeaderParameter::VELOCITY_CHANGE_SPEED * elapsedTime);
		else
			SetVelocity(DesiredVelocity);
	}



	// ---------------------------------------------------------------------- //
	// 隊員に対する更新処理
	// ---------------------------------------------------------------------- //

	// 隊員の陣形所定位置の合計
	float sumUnitDiff = 0.0f;

	// ポーンに近くのポーンを知らせる
	for (int i = 0; i < m_PawnPointers.size(); ++i)
	{
		Pawn* p_TargetPawn = static_cast<Pawn*>(m_PawnPointers[i]);

		for (int j = 0; j < m_PawnPointers.size(); ++j)
		{
			// 自分自身は無視
			if (i == j) continue;

			// 距離を計算して近ければ近くのポーンとして登録
			float distance = (p_TargetPawn->GetPosition() - m_PawnPointers[j]->GetPosition()).Length();
			if (distance < PawnLeaderParameter::NEIGHBOR_DISTANCE)
			{
				p_TargetPawn->AddNeighbor(m_PawnPointers[j]);
			}
		}


		sumUnitDiff += p_TargetPawn->GetDiffToTarget();

		// 現在の陣形安定度を与える
		p_TargetPawn->SetFormationStability(GetFormationStability());
	}

	// 隊員の陣形所定位置の平均を割り出す
	m_AverageUnitDiff = sumUnitDiff / static_cast<float>(m_PawnPointers.size());



	// ---------------------------------------------------------------------- //
	// 陣形安定度の評価
	// ---------------------------------------------------------------------- //

	// 安定度の評価
	float stability = this->GetFormationStability();
	if (m_StabilityState == StabilityStates::Death)
	{
		m_IsBroken = true;
	}
	else if (m_IsBroken || stability < PawnLeaderParameter::StabilityState::BROKEN)
	{
		// 崩れている時間を進める
		if (m_IsBroken) m_BrokenTime += elapsedTime;

		// 陣形は崩れている！！

		m_StabilityState = StabilityStates::Broken;
		m_IsBroken = true;


		// もし、安定度が安定～警告の範囲にあったら、一定時間維持することで崩壊の状態から回復できる
		if (stability > PawnLeaderParameter::StabilityState::BROKEN)
		{
			// 安定度で回復までの時間が反動する
			m_RecoveryTime += stability * elapsedTime;
			if (m_RecoveryTime > PawnLeaderParameter::StabilityState::RECOVERY_TIME)
			{
				m_IsBroken = false;
				m_RecoveryTime = 0.0f;
				m_BrokenTime = 0.0f;
			}
		}
		else
			m_RecoveryTime = 0.0f;


		// もし、陣形所定位置との差分の平均値が上がりすぎると部隊は死ぬ
		if (m_AverageUnitDiff > PawnLeaderParameter::DEATH_LIMIT)
		{
			m_StabilityState = StabilityStates::Death;

			// ゲームデータに「敗北」を送る
			CCC::Managers::GameContextManager::GetInstance()->
				SetGameData("PlayerIsWin", false);

			// リザルトシーンに遷移することを要求
			CCC::Messenger::MessengerHub::GetInstance()->
				Receive(
					CCC::Messenger::MessageType::Request_ResultScene,
					CCC::Messenger::MessengerHub::PayLoad(true)
					);
		}
	}
	else if (stability > PawnLeaderParameter::StabilityState::STABLE)
	{
		m_StabilityState = StabilityStates::Stable;
	}
	else
	{
		m_StabilityState = StabilityStates::Warning;
	}



	// ---------------------------------------------------------------------- //
	// アニメーションの設定
	// ---------------------------------------------------------------------- //

	// アニメーションの設定
	std::string animation;

	// 攻撃中
	if (this->IsAttacking())
	{
		animation = "Paladin_Slash";
	}
	// 方向ベクトルがゼロでなければ移動中
	else if (this->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		// ダッシュ中かどうかでアニメーションを変更
		if (this->IsSkillActive() || mp_InputManager->GetInputAs<bool>("Dash"))
			animation = "Paladin_Run";
		else
			animation = "Paladin_Walk";
	}
	else
	{
		animation = "Paladin_Idle";
	}

	// アニメーションの変更要求
	this->RequestAnimationChange(animation, 0.3f);
}

void PawnLeader::SetCameraTransform(CCC::Bases::ObjectBase* p_Camera)
{
	// オブジェクトからトランスフォームコンポネートを取得する
	mp_CameraTransform = p_Camera->GetComponent<CCC::Components::Transform>("Transform");
}

float PawnLeader::GetFormationStability() const
{
	// もし、安定度の状態が死んでいたら0を返す
	if (m_StabilityState == StabilityStates::Death)
		return 0.0f;

	// もし、安定する境界内なら1を返す
	if (m_AverageUnitDiff <= PawnLeaderParameter::STABLE_LIMIT)
		return 1.0f;

	if (m_SkillState == SkillStates::Active)
	{
		// もし、保てなくなる限界を突破していたら0を返す
		if (m_AverageUnitDiff >= PawnLeaderParameter::BREAK_LIMIT * PawnLeaderParameter::SkillGauge::STABILITY_BUFF)
			return 0.0f;

		// 安定分を引いて、安定度を計算する
		return 1.0f -
			(m_AverageUnitDiff - PawnLeaderParameter::STABLE_LIMIT) /
			(PawnLeaderParameter::BREAK_LIMIT * PawnLeaderParameter::SkillGauge::STABILITY_BUFF - PawnLeaderParameter::STABLE_LIMIT);
	}

	// もし、保てなくなる限界を突破していたら0を返す
	if (m_AverageUnitDiff >= PawnLeaderParameter::BREAK_LIMIT)
		return 0.0f;

	// 安定分を引いて、安定度を計算する
	return 1.0f -
		(m_AverageUnitDiff - PawnLeaderParameter::STABLE_LIMIT) /
		(PawnLeaderParameter::BREAK_LIMIT - PawnLeaderParameter::STABLE_LIMIT);
}



// ---------------------------------------------------------------------- //
// 陣形の設定
// ---------------------------------------------------------------------- //

void PawnLeader::PawnsPositionReset()
{
	for (PawnBase* p_Pawn : m_PawnPointers)
	{
		p_Pawn->SetPosition(static_cast<Pawn*>(p_Pawn)->GetOffset());
	}
}

void PawnLeader::FormationSquare(float between)
{
	// 陣形の一辺を求める
	int size = static_cast<int>(m_PawnPointers.size());
	int length = static_cast<int>(std::sqrtf(static_cast<float>(size)));

	// ポーンのインデックス
	int count = 0;

	for (int i = 1; i <= length; i++)
	{
		for (int j = 1; j <= length; j++)
		{
			// ポインタ－の取得
			Pawn* p_Pawn = static_cast<Pawn*>(m_PawnPointers[count]);

			// 隊列の位置の設定
			p_Pawn->SetOffset(
				DirectX::SimpleMath::Vector3(
					between * static_cast<float>(j) - between * (static_cast<float>(length) * 0.5f) - between * 0.5f,
					0.0f,
					between * static_cast<float>(i))
			);

			// 次のポーンに進む
			count++;
		}
	}

	// あまりがいないなら終了
	if (count >= size) return;

	// 余剰の位置
	float leftover = static_cast<float>(length + 1);

	while (count < size)
	{
		// ポインタ－の取得
		Pawn* p_Pawn = static_cast<Pawn*>(m_PawnPointers[count]);

		// 隊列の位置の設定
		p_Pawn->SetOffset(
			DirectX::SimpleMath::Vector3(
				between * leftover - between * (static_cast<float>(size - count)) - length * between * 0.5f - between * 0.5f,
				0.0f,
				between * leftover)
		);

		// 次のポーンに進む
		count++;
	}
}

void PawnLeader::FormationWedge(float between)
{
	// ポーンのインデックス
	int count = 0;

	// 三角形の一列の人数
	int width = 2;
	// 現在の横の位置
	int widthIndex = 1;

	// ポーンの総数
	int size = static_cast<int>(m_PawnPointers.size());

	// 楔形にならべる
	while (count < size)
	{
		// ポインタ－の取得
		Pawn* p_Pawn = static_cast<Pawn*>(m_PawnPointers[count]);

		// 隊列の位置の設定
		p_Pawn->SetOffset(
			DirectX::SimpleMath::Vector3(
				between * static_cast<float>(widthIndex) - static_cast<float>(width - 1) * between * 0.5f - between,
				0.0f,
				between * static_cast<float>(width - 1)
			)
		);



		// 横の配置のインデックスを進める
		widthIndex++;
		if (widthIndex > width)
		{
			width++;
			widthIndex = 1;

			// 横に十分に並べられなくなったら場合終了
			if (size - count < width)
				break;
		}

		// 次のポーンに進む
		count++;
	}

	// あまったものを配置する
	float leftover = static_cast<float>(size - count);
	while (count < size)
	{
		// ポインタ－の取得
		Pawn* p_Pawn = static_cast<Pawn*>(m_PawnPointers[count]);

		// 隊列の位置の設定
		p_Pawn->SetOffset(
			DirectX::SimpleMath::Vector3(
				between * static_cast<float>(widthIndex) - static_cast<float>(leftover - 1) * between * 0.5f - between,
				0.0f,
				between * static_cast<float>(width - 1)
			)
		);

		// 横の配置のインデックスを進める
		widthIndex++;

		// 次のポーンに進む
		count++;
	}
}

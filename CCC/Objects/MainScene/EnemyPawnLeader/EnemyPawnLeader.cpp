/**
 * @file   EnemyPawnLeader.cpp
 *
 * @brief  敵ポーンリーダーオブジェクトクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 *
 * 敵部隊を指揮する敵ポーンリーダーオブジェクトクラス
 *
 * 2026/01/13
 * 作成
 *
 * 2026/01/18
 * Initialize関数
 * 陣形が死ぬとリザルトシーンに遷移することを要求するように改修
 * 
 * 2026/01/22
 * Initialize関数
 * 状態の変化を受け取るメッセージ登録を追加
 * 
 * Update関数
 * 状態によってターゲットを追跡する処理を追加
 * 
 * 2026/01/23
 * 固有名詞が設定できるようになった
 * 
 * Update関数
 * 状態によってターゲットを攻撃する処理を追加
 * 陣形が死ぬとリザルトシーンに遷移することを要求するよう処理を破棄
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "EnemyPawnLeader.h"

// パラメータ
#include "EnemyPawnLeaderParameter.h"

// コンポーネント
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>
#include <CCC/Components/Colliders/BoxCollider.h>

 // 管理クラス
#include <CCC/Managers/ObjectManager.h>

// メッセンジャー
#include <CCC/Messenger/MessengerHub.h>
#include <CCC/Messenger/MessageType.h>

// 関係のあるオブジェクトクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/MainScene/Pawn.h>
#include <CCC/Objects/PawnManager.h>

// チームIDの数列
#include <CCC/Objects/TeamID.h>

 // ---------------------------------------------------------------------- //
 // パブリック関数
 // ---------------------------------------------------------------------- //

EnemyPawnLeader::EnemyPawnLeader(
	const DirectX::SimpleMath::Vector3& spawnPosition,
	PawnManager*                        p_Manager,
	CCC::Bases::CharacterBase*                   p_Target,
	const std::string&                  name
) :
	PaladinCharacter(TeamID::Enemy, p_Manager),
	mp_PawnManager   (p_Manager),
	m_AverageUnitDiff(0.0f),
	m_StabilityState (StabilityStates::Stable),
	m_IsBroken       (false),
	m_BrokenTime     (0.0f),
	m_RecoveryTime   (0.0f),
	m_SkillState     (SkillStates::Recovering),
	m_SkillGauge     (0.0f),
	m_IsMove         (false),
	m_State          (EnemyPawnLeaderStates::Wait),
	m_Name           (name)
{
	this->SetTarget(p_Target);

	// コンポネートの追加
	this->AddComponent<CCC::Components::PawnCollider>("Collider", this, 1.0f, p_Manager);
	//this->AddComponent<CCC::Components::BoxCollider>("ColliderT", this, this->GetTransform(), false, 3.0f, 3.0f); コライダーのテストのため実装していたもののためコメントアウト

	// 初期生成時の座標を保存
	this->GetComponent<CCC::Components::Transform>("Transform")->SetPosition(spawnPosition);
}

EnemyPawnLeader::~EnemyPawnLeader() = default;

void EnemyPawnLeader::Start()
{
	// ---------------------------------------------------------------------- //
	// 部隊の生成
	// ---------------------------------------------------------------------- //

	// オブジェクト管理クラスの取得
	CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

	for (int i = 0; i < EnemyPawnLeaderParameter::NUMBER_PAWN; i++)
	{
		// ポーンの生成
		CCC::Bases::CharacterBase* p_Pawn = p_om->CreateObject<Pawn>("EnemyPawn" + m_Name + std::to_string(i), TeamID::Enemy, mp_PawnManager);
		p_Pawn->SetTarget(this);

		// ポーンポインタ配列に追加
		m_PawnPointers.push_back(p_Pawn);
	}

	// 陣形の設定
	this->FormationSquare(EnemyPawnLeaderParameter::SPACING_BETWEEN_PAWN);

	// 位置リセット
	this->PawnsPositionReset();



	// ---------------------------------------------------------------------- //
	// メッセージの作成
	// ---------------------------------------------------------------------- //

	// ハブの取得
	CCC::Messenger::MessengerHub* messenger = CCC::Messenger::MessengerHub::GetInstance();

	// ステート変化のメッセージの作成
	messenger->Subscribe(CCC::Messenger::MessageType::RequestToEnemyPawnLeader_State,
		[this](const CCC::Messenger::MessengerHub::PayLoad& is) {
			// 死んでいたらすぐに無視する
			if (m_State == EnemyPawnLeaderStates::Death) return;

			if (const AddressedPayload* p = std::any_cast<AddressedPayload>(&is.item))
			{
				// 宛名が合っているかを確認
				if (p->p_Addressed != this) return;

				// 状態を受け取る
				m_State = p->p_Payload;
			}
		}
	);
}

void EnemyPawnLeader::Process(float elapsedTime)
{
	if (m_State == EnemyPawnLeaderStates::Death) return;

	// ---------------------------------------------------------------------- //
	// 陣形スキル
	// ---------------------------------------------------------------------- //
	switch (m_SkillState)
	{
	case EnemyPawnLeader::SkillStates::Inactive:

		if (m_State == EnemyPawnLeaderStates::Attack && m_StabilityState == StabilityStates::Stable)
		{
			m_SkillState = SkillStates::Active;
			this->FormationWedge(EnemyPawnLeaderParameter::SPACING_BETWEEN_PAWN);
			this->SetIsSkillActive(true);

			float x = this->GetTransform()->GetRotateX();
			m_SkillAngle = DirectX::SimpleMath::Vector3(-std::sinf(x), 0.0f, -std::cosf(x));
		}

		break;
	case EnemyPawnLeader::SkillStates::Active:

		m_SkillGauge -= EnemyPawnLeaderParameter::SkillGauge::CONSUMPTION_RATE * elapsedTime;

		if (m_SkillGauge <= 0.0f || m_StabilityState == StabilityStates::Broken)
		{
			m_SkillGauge = 0.0f;
			m_SkillState = SkillStates::Recovering;
			this->FormationSquare(EnemyPawnLeaderParameter::SPACING_BETWEEN_PAWN);
			this->SetIsSkillActive(false);
		}

		break;
	case EnemyPawnLeader::SkillStates::Recovering:

		m_SkillGauge += EnemyPawnLeaderParameter::SkillGauge::RECOVERY_RATE * this->GetFormationStability() * elapsedTime;

		if (m_SkillGauge >= 1.0f)
		{
			m_SkillGauge = 1.0f;
			m_SkillState = SkillStates::Inactive;
		}

		break;
	case EnemyPawnLeader::SkillStates::Count:
	default:
		// 通常ならありえない処理
		m_SkillState = EnemyPawnLeader::SkillStates::Inactive;
		break;
	}


	// ---------------------------------------------------------------------- //
	// 移動機能
	// ---------------------------------------------------------------------- //

	if (m_State == EnemyPawnLeaderStates::Chase)
	{
		// 目標ベロシティ
		DirectX::SimpleMath::Vector3 DesiredVelocity;

		DesiredVelocity = this->GetTarget()->GetTransform()->GetPosition() - this->GetTransform()->GetPosition();
		DesiredVelocity.Normalize();
		DesiredVelocity *= EnemyPawnLeaderParameter::MOVE_SPEED;

		DirectX::SimpleMath::Vector3 DiffVelocity = DesiredVelocity - this->GetVelocity();

		if (DiffVelocity != DirectX::SimpleMath::Vector3::Zero)
			this->SetVelocity(this->GetVelocity() + DiffVelocity * 0.5f * elapsedTime);
		else
			this->SetVelocity(DesiredVelocity);
	}
	if (m_State == EnemyPawnLeaderStates::Attack)
	{
		// 目標ベロシティ
		DirectX::SimpleMath::Vector3 DesiredVelocity
			= m_SkillAngle * EnemyPawnLeaderParameter::RUN_SPEED;


		DirectX::SimpleMath::Vector3 DiffVelocity = DesiredVelocity - this->GetVelocity();

		if (DiffVelocity != DirectX::SimpleMath::Vector3::Zero)
			this->SetVelocity(this->GetVelocity() + DiffVelocity * EnemyPawnLeaderParameter::SkillGauge::SPEED_BUFF * elapsedTime);
		else
			this->SetVelocity(DesiredVelocity);
	}
	if (m_State == EnemyPawnLeaderStates::Wait)
		SetVelocity(DirectX::SimpleMath::Vector3::Zero);



	// ---------------------------------------------------------------------- //
	// 隊員に対する更新処理
	// ---------------------------------------------------------------------- //

	// 隊員の陣形所定位置の合計
	float sumUnitDiff = 0.0f;

	// ポーンに近くのポーンを知らせる
	for (int i = 0; i < m_PawnPointers.size(); ++i)
	{
		CCC::Bases::CharacterBase* p_TargetPawn = static_cast<CCC::Bases::CharacterBase*>(m_PawnPointers[i]);

		for (int j = 0; j < m_PawnPointers.size(); ++j)
		{
			// 自分自身は無視
			if (i == j) continue;

			// 距離を計算して近ければ近くのポーンとして登録
			float distance = (p_TargetPawn->GetTransform()->GetPosition() - m_PawnPointers[j]->GetTransform()->GetPosition()).Length();
			if (distance < EnemyPawnLeaderParameter::NEIGHBOR_DISTANCE)
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
	else if (m_IsBroken || stability < EnemyPawnLeaderParameter::StabilityState::BROKEN)
	{
		// 崩れている時間を進める
		if (m_IsBroken) m_BrokenTime += elapsedTime;

		// 陣形は崩れている！！

		m_StabilityState = StabilityStates::Broken;
		m_IsBroken = true;


		// もし、安定度が安定～警告の範囲にあったら、一定時間維持することで崩壊の状態から回復できる
		if (stability > EnemyPawnLeaderParameter::StabilityState::BROKEN)
		{
			// 安定度で回復までの時間が反動する
			m_RecoveryTime += stability * elapsedTime;
			if (m_RecoveryTime > EnemyPawnLeaderParameter::StabilityState::RECOVERY_TIME)
			{
				m_IsBroken = false;
				m_RecoveryTime = 0.0f;
				m_BrokenTime = 0.0f;
			}
		}
		else
			m_RecoveryTime = 0.0f;


		// もし、陣形所定位置との差分の平均値が上がりすぎると部隊は死ぬ
		if (m_AverageUnitDiff > EnemyPawnLeaderParameter::DEATH_LIMIT)
		{
			m_StabilityState = StabilityStates::Death;
			m_State = EnemyPawnLeaderStates::Death;

			for (auto* it : m_PawnPointers)
			{
				static_cast<CCC::Bases::CharacterBase*>(it)->RequestStateChange(CCC::StateType::Death);
			}
		}
	}
	else if (stability > EnemyPawnLeaderParameter::StabilityState::STABLE)
	{
		m_StabilityState = StabilityStates::Stable;
	}
	else
	{
		m_StabilityState = StabilityStates::Warning;
	}
}

float EnemyPawnLeader::GetFormationStability() const
{
	if (m_StabilityState == StabilityStates::Death)
		return 0.0f;

	// もし、安定する境界内なら1を返す
	if (m_AverageUnitDiff <= EnemyPawnLeaderParameter::STABLE_LIMIT)
		return 1.0f;

	// もし、保てなくなる限界を突破していたら0を返す
	if (m_AverageUnitDiff >= EnemyPawnLeaderParameter::BREAK_LIMIT)
		return 0.0f;

	// 安定分を引いて、安定度を計算する
	return 1.0f -
		(m_AverageUnitDiff - EnemyPawnLeaderParameter::STABLE_LIMIT) /
		(EnemyPawnLeaderParameter::BREAK_LIMIT - EnemyPawnLeaderParameter::STABLE_LIMIT);
}



// ---------------------------------------------------------------------- //
// 陣形の設定
// ---------------------------------------------------------------------- //

void EnemyPawnLeader::PawnsPositionReset()
{
	for (CCC::Bases::CharacterBase* p_Pawn : m_PawnPointers)
	{
		p_Pawn->GetTransform()->SetPosition(this->GetTransform()->GetPosition() + static_cast<CCC::Bases::CharacterBase*>(p_Pawn)->GetOffset());
	}
}

void EnemyPawnLeader::FormationSquare(float between)
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
			CCC::Bases::CharacterBase* p_Pawn = static_cast<CCC::Bases::CharacterBase*>(m_PawnPointers[count]);

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
		CCC::Bases::CharacterBase* p_Pawn = static_cast<CCC::Bases::CharacterBase*>(m_PawnPointers[count]);

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

void EnemyPawnLeader::FormationWedge(float between)
{
	int count      = 0;											// ポーンのインデックス
	int width      = 2;											// 三角形の一列の人数
	int widthIndex = 1;											// 現在の横の位置
	int size       = static_cast<int>(m_PawnPointers.size());	// ポーンの総数

	// 楔形にならべる
	while (count < size)
	{
		// ポインタ－の取得
		CCC::Bases::CharacterBase* p_Pawn = static_cast<CCC::Bases::CharacterBase*>(m_PawnPointers[count]);

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
		CCC::Bases::CharacterBase* p_Pawn = static_cast<CCC::Bases::CharacterBase*>(m_PawnPointers[count]);

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

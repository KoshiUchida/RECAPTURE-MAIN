/**
 * @file   Pawn.cpp
 *
 * @brief  ポーンオブジェクトクラスのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/05
 *
 * 2025/12/17
 * 実装
 *
 * 2025/12/19
 * ステアリングビヘイビアーの処理を追加
 * 
 * 2025/12/21
 * ステアリングビヘイビアーの処理を改修
 * 重複していた処理を更新処理に移動
 * 
 * 2026/01/05
 * 陣形の所定の位置との差分を計算して保持する機能の実装
 */

#include "pch.h"
#include "Pawn.h"
#include "PawnParameter.h"

 // コンポネート
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

#include <CCC/Managers/RandomManager.h>

#include <CCC/Common/Easings.h>

#include "States/PawnIdle.h"
#include "States/PawnMove.h"
#include "States/PawnAttack.h"
#include "States/PawnKnockback.h"
#include "States/PawnStandUp.h"
#include "States/PawnDeath.h"

Pawn::Pawn(const TeamID& teamID, PawnManager* p_PawnManager) :
	PawnBase(teamID),
	m_Speed(PawnParameter::MAX_SPEED),
	m_Theta(0.0f),
	mp_Target(nullptr),
	m_IsRunning(false),
	m_DiffToTarget(0.0f),
	m_FormationStability(1.0f)
{
	this->AddComponent<CCC::Components::PawnCollider>("Collider", this, 1.0f, p_PawnManager);
}

Pawn::~Pawn() = default;

void Pawn::Initialize()
{
	// モデルステートにアニメーションプレイヤーを追加
	AddAnimationPlayer("Paladin_Idle");
	AddAnimationPlayer("Paladin_Walk");
	AddAnimationPlayer("Paladin_Run");
	AddAnimationPlayer("Paladin_Slash");
	AddAnimationPlayer("Paladin_FallingBackDeath");
	AddAnimationPlayer("Paladin_StandUp");
	AddAnimationPlayer("Paladin_Death");

	// サイズを0.02に設定
	SetScale(0.02);

	// 初期ベロシティを-1radに
	SetVelocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f));

	// ステートファクトリの登録
	m_Factories["Idle"] = [this]() {
		return std::make_unique<PawnIdle>(this);
		};

	m_Factories["Move"] = [this]() {
		return std::make_unique<PawnMove>(this);
		};

	m_Factories["Attack"] = [this]() {
		return std::make_unique<PawnAttack>(this);
		};

	m_Factories["Knockback"] = [this]() {
		return std::make_unique<PawnKnockback>(this);
		};

	m_Factories["StandUp"] = [this]() {
		return std::make_unique<PawnStandUp>(this);
		};

	m_Factories["Death"] = [this]() {
		return std::make_unique<PawnDeath>(this);
		};

	// ステートマシンの初期化
	m_State = m_Factories["Idle"]();
}

void Pawn::Process(float elapsedTime)
{

	// ---------------------------------------------------------------------- //
	// 陣形の所定の位置との差分を求める
	// ---------------------------------------------------------------------- //

	// ターゲットから座標と回転を取得
	DirectX::SimpleMath::Vector3    targetPosition = mp_Target->GetPosition();
	DirectX::SimpleMath::Quaternion targetRotation = mp_Target->GetTransform()->GetQuaternion();

	// オフセットにターゲットの回転を適用
	DirectX::SimpleMath::Vector3 rotatedOffset = DirectX::SimpleMath::Vector3::Transform(m_Offset, targetRotation);

	// 所定の位置を計算
	targetPosition += rotatedOffset;

	// 現在の座標との差分を保存する
	m_DiffToTarget = (targetPosition - GetPosition()).Length();

	DirectX::SimpleMath::Vector3 outSide = this->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
	if (outSide != DirectX::SimpleMath::Vector3::Zero)
	{
		this->SetVelocity(outSide);
		this->RequestStateChange("Knockback");
	}

	
	
	// ---------------------------------------------------------------------- //
	// ステートの更新処理
	// ---------------------------------------------------------------------- //
	
	// ステートの更新処理
	m_State->Update(elapsedTime);

	// リクエストがあればステート変更
	if (m_RequestState.empty() == false)
	{
		m_State.reset();
		m_State = m_Factories[m_RequestState]();
		m_RequestState.clear();
	}

	ResetNeighbors();
}

float Pawn::GetMaxSpeed()
{
	float speed = 0.0f;

	if (m_IsRunning)
		speed = PawnParameter::MAX_RUN_SPEED;
	else
		speed = PawnParameter::MAX_SPEED;

	if (mp_Target && mp_Target->IsSkillActive())
		speed *= PawnParameter::SPEED_BUFF;

	return speed;
}

DirectX::SimpleMath::Vector3 Pawn::Seek(const DirectX::SimpleMath::Vector3& targetPosition)
{
	// 目標ベロシティ
	DirectX::SimpleMath::Vector3 DesiredVelocity;

	DesiredVelocity = targetPosition - GetPosition();
	DesiredVelocity.Normalize();
	DesiredVelocity *= GetMaxSpeed();

	return DesiredVelocity - GetVelocity();
}

DirectX::SimpleMath::Vector3 Pawn::Flee(const DirectX::SimpleMath::Vector3& targetPosition)
{
	DirectX::SimpleMath::Vector3 DesiredVelocity;

	DesiredVelocity = GetPosition() - targetPosition;
	DesiredVelocity.Normalize();
	DesiredVelocity *= GetMaxSpeed();

	return DesiredVelocity - GetVelocity();

}

DirectX::SimpleMath::Vector3 Pawn::Arrive(const DirectX::SimpleMath::Vector3& targetPosition)
{
	// 目標ベロシティ
	DirectX::SimpleMath::Vector3 DesiredVelocity;

	DesiredVelocity = targetPosition - GetPosition();

	float distance = DesiredVelocity.Length();

	m_Speed = GetMaxSpeed();
	if (distance <= PawnParameter::SLOWING_RADIUS)
	{
		float deceleration = CCC::Ease::InCirc(distance / PawnParameter::SLOWING_RADIUS);

		m_Speed *= deceleration;
	}


	DesiredVelocity.Normalize();



	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	velocity.Normalize();

	DirectX::SimpleMath::Vector3 ahead = GetPosition() + velocity * PawnParameter::MAX_SEE_AHEAD;
	//DirectX::SimpleMath::Vector3 ahead2 = ahead * 0.5f;

	DirectX::SimpleMath::Vector3 most = DirectX::SimpleMath::Vector3::Zero;
	for (auto* p_Neighbor : m_Neighbors)
	{
		float neighborDistance = (p_Neighbor->GetPosition() - ahead).Length();
		//float neighborDistance2 = (p_Neighbor->GetPosition() - ahead2).Length();
		if (neighborDistance < PawnParameter::MAX_SEE_AHEAD)
		{
			DirectX::SimpleMath::Vector3 avoidance_force = ahead - p_Neighbor->GetPosition();
			avoidance_force.Normalize();
			avoidance_force *= PawnParameter::MAX_AVOID_FORCE;
		}
	}

	most.Normalize();

	DesiredVelocity += most;

	DesiredVelocity *= m_Speed;

	return DesiredVelocity - GetVelocity();
}

DirectX::SimpleMath::Vector3 Pawn::Pursuit(const DirectX::SimpleMath::Vector3& targetPosition, const DirectX::SimpleMath::Vector3& targetVelocity)
{
	DirectX::SimpleMath::Vector3 FutureTarget =
		targetPosition + targetVelocity * PawnParameter::PREDICTION_TIME;

	return FutureTarget;
}

DirectX::SimpleMath::Vector3 Pawn::Wander()
{
	DirectX::SimpleMath::Vector3 NormalVelocity = GetVelocity();
	NormalVelocity.Normalize();

	DirectX::SimpleMath::Vector3 circle_center = GetPosition() + NormalVelocity * PawnParameter::CIRCLE_DISTANCE;
	
	DirectX::SimpleMath::Vector3 wander_offset =
		DirectX::SimpleMath::Vector3(cosf(m_Theta), 0.0f, sinf(m_Theta)) * PawnParameter::CIRCLE_RADIUS;
	
	CCC::Managers::RandomManager* p_rm = CCC::Managers::RandomManager::GetInstance();

	m_Theta += p_rm->Rand<float>(-PawnParameter::WANDER_JITTER, PawnParameter::WANDER_JITTER);

	return circle_center + wander_offset;
}

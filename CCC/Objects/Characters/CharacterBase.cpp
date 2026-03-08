/**
 * @file   CharacteBase.cpp
 *
 * @brief  キャラクター基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 * キャラクタークラスの基底クラス
 * ステートの管理・運用、アニメーションの管理・運用
 * 継承する場合、必ずアニメーション初期化関数をオーバーライドし、
 * モデルとアニメーション、アニメーションの種別ごとのアニメーション名の登録をしなければならない
 *
 * 2026/03/08
 * 作成
 */

// プリコンパイル済みヘッダーファイル
#include "pch.h"

// クラス定義元
#include "CharacterBase.h"

// パラメータ
#include "Paladin/PaladinParameter.h"

// イージング関数
#include <CCC/Common/Easings.h>

// 管理クラス
#include <CCC/Managers/CameraManager.h>
#include <CCC/Managers/RandomManager.h>
#include <CCC/Objects/PawnManager.h>

// コンポネート
#include <CCC/Components/Transform.h>
#include <CCC/Objects/PawnCollider.h>

// ステート
#include "States/IdleState.h"
#include "States/ChaseState.h"
#include "States/MoveState.h"
#include "States/AttackState.h"
#include "States/KnockbackState.h"
#include "States/StandUpState.h"
#include "States/DeathState.h"
#include "States/BlockState.h"

namespace CCC::Bases
{
	CharacterBase::CharacterBase(const TeamID& teamID, PawnManager* p_PawnManager) :
		ObjectBase(),
		mp_Transform(nullptr),
		m_Velocity(DirectX::SimpleMath::Vector3::Zero),
		m_RequestAnimation(std::string()),
		m_IsSkillActive(false),
		m_IsAttacking(false),
		m_TeamID(teamID),
		m_Speed(0.0f),
		mp_Target(nullptr),
		m_Theta(0.0f),
		m_Offset(DirectX::SimpleMath::Vector3::Zero),
		m_IsRunning(false),
		m_StateName(CCC::StateType::None),
		m_RequestState(CCC::StateType::None),
		m_DiffToTarget(0.0f),
		m_HP(1.0f),
		m_FormationStability(1.0f)
	{
		// コンポネートの生成
		// トランスフォーム
		mp_Transform = this->AddComponent<CCC::Components::Transform>("Transform", this);

		// コライダー
		this->AddComponent<CCC::Components::PawnCollider>("Collider", this, 1.0f, p_PawnManager);
	}

	CharacterBase::~CharacterBase() = default;

	void CharacterBase::Initialize()
	{
		// 継承先でのアニメーションの初期化設定
		this->InitializeAnimationState();

		// 初期ベロシティを-1radに
		SetVelocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f));

		// ステートファクトリの登録
		using CCC::StateType;

		m_Factories[StateType::Idle] = [this]() {
			return std::make_unique<CCC::States::IdleState>(this);
			};

		m_Factories[StateType::Chase] = [this]() {
			return std::make_unique<CCC::States::ChaseState>(this);
			};

		m_Factories[StateType::Move] = [this]() {
			return std::make_unique<CCC::States::MoveState>(this);
			};

		m_Factories[StateType::Attack] = [this]() {
			return std::make_unique<CCC::States::AttackState>(this);
			};

		m_Factories[StateType::Knockback] = [this]() {
			return std::make_unique<CCC::States::KnockbackState>(this);
			};

		m_Factories[StateType::StandUp] = [this]() {
			return std::make_unique<CCC::States::StandUpState>(this);
			};

		m_Factories[StateType::Block] = [this]() {
			return std::make_unique<CCC::States::BlockState>(this);
			};

		m_Factories[StateType::Death] = [this]() {
			return std::make_unique<CCC::States::DeathState>(this);
			};

		// ステートマシンの初期化
		m_StateName = StateType::Idle;
		m_State = m_Factories[m_StateName]();

		// 継承先の初期化処理
		this->Start();
	}

	void CharacterBase::Update(float elapsedTime)
	{

		// ---------------------------------------------------------------------- //
		// 陣形の所定の位置との差分を求める
		// ---------------------------------------------------------------------- //
		if (mp_Target)
		{
			// ターゲットから座標と回転を取得
			DirectX::SimpleMath::Vector3    targetPosition = mp_Target->GetTransform()->GetPosition();
			DirectX::SimpleMath::Quaternion targetRotation = mp_Target->GetTransform()->GetQuaternion();

			// オフセットにターゲットの回転を適用
			DirectX::SimpleMath::Vector3 rotatedOffset = DirectX::SimpleMath::Vector3::Transform(m_Offset, targetRotation);

			// 所定の位置を計算
			targetPosition += rotatedOffset;

			// 現在の座標との差分を保存する
			m_DiffToTarget = (targetPosition - mp_Transform->GetPosition()).Length();
		}


		// ---------------------------------------------------------------------- //
		// 継承先の更新処理
		// ---------------------------------------------------------------------- //
		this->Process(elapsedTime);



		// ---------------------------------------------------------------------- //
		// ステートの更新処理
		// ---------------------------------------------------------------------- //

		// ステートの更新処理
		m_State->Update(elapsedTime);

		// リクエストがあればステート変更
		if (m_RequestState != CCC::StateType::None)
		{
			m_State.reset();
			m_StateName = m_RequestState;
			m_State = m_Factories[m_StateName]();
			m_RequestState = CCC::StateType::None;
		}

		m_Neighbors.clear();


		// ベロシティがゼロでなければ移動処理を行う
		if (m_Velocity != DirectX::SimpleMath::Vector3::Zero)
		{
			DirectX::SimpleMath::Vector3 normalizedDirection = m_Velocity;
			normalizedDirection.Normalize();

			mp_Transform->SetRotateX(FastAtan2(normalizedDirection.z, -normalizedDirection.x));
			mp_Transform->AddRotateX(DirectX::XM_PIDIV2);
			mp_Transform->AddPosition(m_Velocity * elapsedTime);
		}

		// アニメーションステートの更新処理を行う
		m_AnimationState->Update(elapsedTime);
	}

	void CharacterBase::Render()
	{
		// カメラ管理クラスの取得
		CCC::Managers::CameraManager* p_cm = CCC::Managers::CameraManager::GetInstance();

		// アニメーションステートの描画処理を行う
		m_AnimationState->Render(mp_Transform->GetWorldMatrix(), p_cm->GetView());
	}

	void CharacterBase::Finalize()
	{
	}

	void CharacterBase::RequestStateChange(const CCC::StateType& state)
	{
		m_RequestState = state;
	}

	void CharacterBase::AddVelocity(const DirectX::SimpleMath::Vector3& vel)
	{
		m_Velocity += vel;
	}

	void CharacterBase::AddNeighbor(CharacterBase* nearObject)
	{
		m_Neighbors.push_back(nearObject);
	}

	DirectX::SimpleMath::Vector3 CharacterBase::Seek(const DirectX::SimpleMath::Vector3& targetPosition)
	{
		// 目標ベロシティ
		DirectX::SimpleMath::Vector3 DesiredVelocity;

		DesiredVelocity = targetPosition - this->GetTransform()->GetPosition();
		DesiredVelocity.Normalize();
		DesiredVelocity *= GetMaxSpeed();

		return DesiredVelocity - GetVelocity();
	}

	DirectX::SimpleMath::Vector3 CharacterBase::Flee(const DirectX::SimpleMath::Vector3& targetPosition)
	{
		DirectX::SimpleMath::Vector3 DesiredVelocity;

		DesiredVelocity = this->GetTransform()->GetPosition() - targetPosition;
		DesiredVelocity.Normalize();
		DesiredVelocity *= GetMaxSpeed();

		return DesiredVelocity - GetVelocity();

	}

	DirectX::SimpleMath::Vector3 CharacterBase::Arrive(const DirectX::SimpleMath::Vector3& targetPosition)
	{
		// 目標ベロシティ
		DirectX::SimpleMath::Vector3 DesiredVelocity;

		DesiredVelocity = targetPosition - this->GetTransform()->GetPosition();

		float distance = DesiredVelocity.Length();

		m_Speed = GetMaxSpeed();
		if (distance <= CCC::PaladinParamerter::SLOWING_RADIUS)
		{
			float deceleration = CCC::Ease::InCirc(distance / CCC::PaladinParamerter::SLOWING_RADIUS);

			m_Speed *= deceleration;
		}


		DesiredVelocity.Normalize();



		DirectX::SimpleMath::Vector3 velocity = GetVelocity();
		velocity.Normalize();

		DirectX::SimpleMath::Vector3 ahead = this->GetTransform()->GetPosition() + velocity * CCC::PaladinParamerter::MAX_SEE_AHEAD;

		DirectX::SimpleMath::Vector3 most = DirectX::SimpleMath::Vector3::Zero;
		for (auto* p_Neighbor : m_Neighbors)
		{
			float neighborDistance = (p_Neighbor->GetTransform()->GetPosition() - ahead).Length();
			if (neighborDistance < CCC::PaladinParamerter::MAX_SEE_AHEAD)
			{
				DirectX::SimpleMath::Vector3 avoidance_force = ahead - p_Neighbor->GetTransform()->GetPosition();
				avoidance_force.Normalize();
				avoidance_force *= CCC::PaladinParamerter::MAX_AVOID_FORCE;
			}
		}

		most.Normalize();

		DesiredVelocity += most;

		DesiredVelocity *= m_Speed;

		return DesiredVelocity - this->GetVelocity();
	}

	DirectX::SimpleMath::Vector3 CharacterBase::Pursuit(const DirectX::SimpleMath::Vector3& targetPosition, const DirectX::SimpleMath::Vector3& targetVelocity)
	{
		DirectX::SimpleMath::Vector3 FutureTarget =
			targetPosition + targetVelocity * CCC::PaladinParamerter::PREDICTION_TIME;

		return FutureTarget;
	}

	DirectX::SimpleMath::Vector3 CharacterBase::Wander()
	{
		DirectX::SimpleMath::Vector3 NormalVelocity = this->GetVelocity();
		NormalVelocity.Normalize();

		DirectX::SimpleMath::Vector3 circle_center = this->GetTransform()->GetPosition() + NormalVelocity * CCC::PaladinParamerter::CIRCLE_DISTANCE;

		DirectX::SimpleMath::Vector3 wander_offset =
			DirectX::SimpleMath::Vector3(cosf(m_Theta), 0.0f, sinf(m_Theta)) * CCC::PaladinParamerter::CIRCLE_RADIUS;

		CCC::Managers::RandomManager* p_rm = CCC::Managers::RandomManager::GetInstance();

		m_Theta += p_rm->Rand<float>(-CCC::PaladinParamerter::WANDER_JITTER, CCC::PaladinParamerter::WANDER_JITTER);

		return circle_center + wander_offset;
	}



	// ---------------------------------------------------------------------- //
	// 設定関数
	// ---------------------------------------------------------------------- //

	void CharacterBase::SetVelocity(const DirectX::SimpleMath::Vector3& velocity)
	{
		m_Velocity = velocity;
	}

	void CharacterBase::SetOffset(const DirectX::SimpleMath::Vector3& offset)
	{
		m_Offset = offset;
	}

	void CharacterBase::SetFormationStability(float stability)
	{
		m_FormationStability = stability;
	}

	void CharacterBase::SetRunning(bool is)
	{
		m_IsRunning = is;
	}

	void CharacterBase::SetIsSkillActive(bool is)
	{
		m_IsSkillActive = is;
	}

	void CharacterBase::SetTarget(CharacterBase* target)
	{
		mp_Target = target;
	}



	// ---------------------------------------------------------------------- //
	// 取得関数
	// ---------------------------------------------------------------------- //

	DirectX::SimpleMath::Vector3 CharacterBase::GetVelocity() const
	{
		return m_Velocity;
	}

	DirectX::SimpleMath::Vector3 CharacterBase::GetOffset() const
	{
		return m_Offset;
	}

	float CharacterBase::GetMaxSpeed() const
	{
		float speed = 0.0f;

		if (m_IsRunning)
			speed = CCC::PaladinParamerter::MAX_RUN_SPEED;
		else
			speed = CCC::PaladinParamerter::MAX_SPEED;

		if (mp_Target && mp_Target->IsSkillActive())
			speed *= CCC::PaladinParamerter::SPEED_BUFF;

		return speed;
	}

	float CharacterBase::GetHP() const
	{
		return m_HP;
	}

	float CharacterBase::GetFormationStability() const
	{
		return m_FormationStability;
	}

	float CharacterBase::GetDiffToTarget() const
	{
		return m_DiffToTarget;
	}

	bool CharacterBase::IsSkillActive() const
	{
		return m_IsSkillActive;
	}

	bool CharacterBase::IsRunning() const
	{
		return m_IsRunning;
	}

	TeamID CharacterBase::GetTeamID() const
	{
		return m_TeamID;
	}

	CCC::StateType CharacterBase::GetState() const
	{
		return m_StateName;
	}

	CCC::Components::Transform* CharacterBase::GetTransform()
	{
		return mp_Transform;
	}

	CCC::Animations::AnimationStateBace* CharacterBase::GetAnimationState()
	{
		return m_AnimationState.get();
	}

	CharacterBase* CharacterBase::GetTarget()
	{
		return mp_Target;
	}

	std::string CharacterBase::GetAnimationTypeName(const AnimationType& type)
	{
		return m_AnimationTypes[type];
	}



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //

	float CharacterBase::FastAtan2(float y, float x) const
	{
		const float ONEQTR_PI = 0.78539816339f;
		const float THRQTR_PI = 2.35619449019f;

		float abs_y = fabsf(y) + 1e-10f;
		float angle;

		if (x < 0.0f) {
			float r = (x + abs_y) / (abs_y - x);
			angle = THRQTR_PI;
			angle += (0.1963f * r * r - 0.9817f) * r;
		}
		else {
			float r = (x - abs_y) / (x + abs_y);
			angle = ONEQTR_PI;
			angle += (0.1963f * r * r - 0.9817f) * r;
		}

		return y < 0.0f ? -angle : angle;
	}

	void CharacterBase::CreateAnimationState(const std::string& model, const std::string& firstState)
	{
		// モデルステートの生成
		m_AnimationState = std::make_unique<CCC::Animations::AnimationStateBace>(model, firstState);
	}

	void CharacterBase::AddAnimationTypeName(const AnimationType& type, const std::string& animation)
	{
		m_AnimationTypes[type] = animation;
	}
}

/**
 * @file   CharacteBase.h
 *
 * @brief  キャラクター基底クラスのヘッダファイル
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

// 多重インクルードガード
#pragma once

// 基底クラス
#include <CCC/Objects/ObjectBase.h>

#include <CCC/Objects/TeamID.h>

// アニメーションのステートマシン
#include <CCC/Animation/AnimationStateBace.h>
#include <CCC/Objects/Characters/AnimationType.h>

// ステートマシーン
#include <CCC/Interfaces/IState.h>
#include "StateType.h"


class PawnManager;
namespace CCC
{
	namespace Components
	{
		class Transform;
	}

	namespace Bases
	{
		class CharacterBase :
			public CCC::Bases::ObjectBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="teamID">チームID</param>
			/// <param name="p_PawnManager">ポーンの管理クラス</param>
			CharacterBase(const TeamID& teamID, PawnManager* p_PawnManager);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~CharacterBase();

			/// <summary>
			/// 初期化処理
			/// </summary>
			void Initialize() override final;

			/// <summary>
			/// 継承先別の初期化処理
			/// </summary>
			virtual void Start() = 0;

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime) override final;

			/// <summary>
			/// 継承先別の更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			virtual void Process(float elapsedTime) = 0;

			/// <summary>
			/// 描画処理
			/// </summary>
			void Render() override final;

			/// <summary>
			/// 終了処理
			/// </summary>
			void Finalize()  override final;

			/// <summary>
			/// ステート変更リクエスト
			/// </summary>
			/// <param name="state">変更先のステート</param>
			void RequestStateChange(const CCC::StateType& state);

			/// <summary>
			/// ベロシティの加算
			/// </summary>
			void AddVelocity(const DirectX::SimpleMath::Vector3& vel);

			/// <summary>
			/// 近くにいるオブジェクトを追加
			/// </summary>
			void AddNeighbor(CharacterBase* nearObject);



			// ---------------------------------------------------------------------- //
			// ステアリングビヘイビアー
			// ---------------------------------------------------------------------- //
			DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPosition);
			DirectX::SimpleMath::Vector3 Flee(const DirectX::SimpleMath::Vector3& targetPosition);
			DirectX::SimpleMath::Vector3 Arrive(const DirectX::SimpleMath::Vector3& targetPosition);
			DirectX::SimpleMath::Vector3 Pursuit(const DirectX::SimpleMath::Vector3& targetPosition, const DirectX::SimpleMath::Vector3& targetVelocity);
			DirectX::SimpleMath::Vector3 Wander();



			// ---------------------------------------------------------------------- //
			// 設定関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// ベロシティの設定
			/// </summary>
			void SetVelocity(const DirectX::SimpleMath::Vector3& velocity);

			/// <summary>
			/// オフセットの設定
			/// </summary>
			void SetOffset(const DirectX::SimpleMath::Vector3& offset);

			/// <summary>
			/// 陣形安定度の設定
			/// </summary>
			void SetFormationStability(float stability);

			/// <summary>
			/// 走っているかの設定
			/// </summary>
			void SetRunning(bool is);

			/// <summary>
			/// スキルが発動中かの設定
			/// </summary>
			void SetIsSkillActive(bool is);

			/// <summary>
			/// ターゲットの設定
			/// </summary>
			void SetTarget(CharacterBase* target);



			// ---------------------------------------------------------------------- //
			// 取得関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// ベロシティの取得
			/// </summary>
			DirectX::SimpleMath::Vector3 GetVelocity() const;

			/// <summary>
			/// オフセットの取得
			/// </summary>
			DirectX::SimpleMath::Vector3 GetOffset() const;

			/// <summary>
			/// 速度の取得
			/// </summary>
			float GetMaxSpeed() const;

			/// <summary>
			/// HPの取得
			/// </summary>
			float GetHP() const;

			/// <summary>
			/// 陣形安定度の取得
			/// </summary>
			virtual float GetFormationStability() const;

			/// <summary>
			/// ターゲットとの差分
			/// </summary>
			float GetDiffToTarget() const;

			/// <summary>
			/// スキル発動中かの取得
			/// </summary>
			bool IsSkillActive() const;

			/// <summary>
			/// 走っているかの取得
			/// </summary>
			bool IsRunning() const;

			/// <summary>
			/// チームIDの取得
			/// </summary>
			TeamID GetTeamID() const;

			/// <summary>
			/// 現在のステートを取得する
			/// </summary>
			CCC::StateType GetState() const;

			/// <summary>
			/// トランスフォームの取得
			/// </summary>
			CCC::Components::Transform* GetTransform();

			/// <summary>
			/// アニメーションステートマシンの取得
			/// </summary>
			CCC::Animations::AnimationStateBace* GetAnimationState();

			/// <summary>
			/// ターゲットの取得
			/// </summary>
			CharacterBase* GetTarget();

			/// <summary>
			/// アニメーションの種別に登録したアニメーションのリソース登録名の取得
			/// </summary>
			std::string GetAnimationTypeName(const AnimationType& type);



			// ---------------------------------------------------------------------- //
			// メンバ関数
			// ---------------------------------------------------------------------- //
		private:
			/// <summary>
			/// 計算を大雑把にしたAtan2
			/// </summary>
			float FastAtan2(float y, float x) const;

		protected:
			/// <summary>
			/// アニメーション関連の初期化
			/// </summary>
			virtual void InitializeAnimationState() = 0;

			/// <summary>
			/// 継承先限定のアニメーションステート生成
			/// </summary>
			/// <param name="model">モデルのリソース登録名</param>
			/// <param name="firstState">最初のアニメーションステートの登録名</param>
			void CreateAnimationState(const std::string& model, const std::string& firstState);

			/// <summary>
			/// アニメーションの種別に沿ったアニメーションのリソース登録名の追加
			/// </summary>
			/// <param name="type">アニメーションの種別</param>
			/// <param name="animation">アニメーションのリソース登録名</param>
			void AddAnimationTypeName(const AnimationType& type, const std::string& animation);



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// トランスフォーム
			CCC::Components::Transform* mp_Transform;

			// 現在のベロシティ
			DirectX::SimpleMath::Vector3 m_Velocity;

			// モデルステート
			std::unique_ptr<CCC::Animations::AnimationStateBace> m_AnimationState;

			// リクエストアニメーション
			std::string m_RequestAnimation;

			// アニメーションの名前
			using AnimationTypes = std::unordered_map<CCC::AnimationType, std::string>;
			AnimationTypes m_AnimationTypes;

			// 現在スキル発動中か
			bool m_IsSkillActive;

			// 現在攻撃中か
			bool m_IsAttacking;

			// 自身のチーム
			TeamID m_TeamID;

			// 速度
			float m_Speed;

			// ターゲット
			CharacterBase* mp_Target;

			// ランダム角度
			float m_Theta;

			// 隊列の位置
			DirectX::SimpleMath::Vector3 m_Offset;

			// 今走るか?
			bool m_IsRunning;

			// 近くにいる他のポーンベースの全て
			std::vector<CharacterBase*> m_Neighbors;

			// シーンの生成関数を保持するマップ
			using StateFactories = std::unordered_map<CCC::StateType, std::function<std::unique_ptr<CCC::Interfaces::IState>()>>;
			StateFactories m_Factories;

			// 現在のステート
			CCC::StateType m_StateName;
			std::unique_ptr<CCC::Interfaces::IState> m_State;

			// リクエストステート
			CCC::StateType m_RequestState;

			// 目標との差分
			float m_DiffToTarget;

			// ヘルスポイント
			float m_HP;

			// 陣形安定度
			float m_FormationStability;
		};
	}
}

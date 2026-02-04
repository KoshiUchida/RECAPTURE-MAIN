/**
 * @file   PawnBase.h
 *
 * @brief  ポーン基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/07
 * 2026/01/07
 * スキル発動中かの機能を実装
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include "ObjectBase.h"

// アニメーションのステートマシン
#include <CCC/Animation/AnimationStateBace.h>

// チームIDの数列
#include "TeamID.h"

namespace CCC
{
	// 前方宣言
	namespace Components
	{
		class Transform;
	}

	namespace Bases
	{
		/// <summary>
		/// ポーンの基底オブジェクトクラス
		/// </summary>
		class PawnBase :
			public ObjectBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="teamID">チームID</param>
			PawnBase(const TeamID& teamID);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~PawnBase();

			/// <summary>
			/// 初期化処理
			/// </summary>
			virtual void Initialize() = 0;

			/// <summary>
			/// 子クラスの更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			virtual void Process(float elapsedTime) = 0;

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime) override final;

			/// <summary>
			/// 描画処理
			/// </summary>
			void Render() override final;

			/// <summary>
			/// 終了処理
			/// </summary>
			void Finalize() override final;

			void RequestAnimationChange(
				const std::string& animationName,
				float blendTime)
			{
				m_AnimationState->RequestAnimationChange(animationName, blendTime);
			}

			void ResetAnimationSpeed();

			std::string GetCurrentAnimationName() const;


			bool EndCurrentAnimation() const;

			void SetAnimationTime(float time);

			float GetCurrentAnimationDuration() const;



		private:
			// トランスフォーム
			CCC::Components::Transform* mp_Transform;

			// 現在のベロシティ
			DirectX::SimpleMath::Vector3 m_Velocity;

			// モデルステート
			std::unique_ptr<CCC::Animations::AnimationStateBace> m_AnimationState;
			
			// リクエストアニメーション
			std::string m_RequestAnimation;

			// 現在スキル発動中か
			bool m_IsSkillActive;

			// 現在攻撃中か
			bool m_IsAttacking;

			// 自身のチーム
			TeamID m_TeamID;



		public:
			virtual void SetPosition(const DirectX::SimpleMath::Vector3& position);

			void SetRotate(const DirectX::SimpleMath::Vector3& rotate);

			void SetScale(float scale);

			void SetVelocity(const DirectX::SimpleMath::Vector3& velocity);

			void SetAnimationSpeed(float speed);

			void SetIsSkillActive(bool is)
			{
				m_IsSkillActive = is;
			}

			void SetIsAttack(bool is)
			{
				m_IsAttacking = is;
			}



		public:
			CCC::Components::Transform* GetTransform() const;

			DirectX::SimpleMath::Vector3 GetPosition() const;

			DirectX::SimpleMath::Vector3 GetVelocity() const;

			TeamID GetTeamID()const {
				return m_TeamID;
			}

			bool IsSkillActive() const {
				return m_IsSkillActive;
			}

			bool IsAttacking() const {
				return m_IsAttacking;
			}


		public:
			void AddPosition(const DirectX::SimpleMath::Vector3& position);

			void AddAnimationPlayer(const std::string& animationName);

			void AddVelocity(const DirectX::SimpleMath::Vector3& velocity);
		};
	}
}

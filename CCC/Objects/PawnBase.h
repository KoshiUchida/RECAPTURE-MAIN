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

#pragma once
#include "ObjectBase.h"
#include <CCC/Animation/AnimationStateBace.h>

namespace CCC
{
	// 前方宣言
	namespace Components
	{
		class Transform;
	}

	namespace Bases
	{
		class PawnBase :
			public ObjectBase
		{
		public:
			PawnBase();
			virtual ~PawnBase();

			virtual void Initialize() = 0;
			virtual void Process(float elapsedTime) = 0;
			void Update(float elapsedTime) override final;
			void Render() override final;
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



		public:
			void SetPosition(const DirectX::SimpleMath::Vector3& position);

			void SetRotate(const DirectX::SimpleMath::Vector3& rotate);

			void SetScale(float scale);

			void SetVelocity(const DirectX::SimpleMath::Vector3& velocity);

			void SetAnimationSpeed(float speed);

			void SetIsSkillActive(bool is)
			{
				m_IsSkillActive = is;
			}



		public:
			CCC::Components::Transform* GetTransform() const;

			DirectX::SimpleMath::Vector3 GetPosition() const;

			DirectX::SimpleMath::Vector3 GetVelocity() const;

			bool IsSkillActive() const {
				return m_IsSkillActive;
			}


		public:
			void AddPosition(const DirectX::SimpleMath::Vector3& position);

			void AddAnimationPlayer(const std::string& animationName);

			void AddVelocity(const DirectX::SimpleMath::Vector3& velocity);
		};
	}
}

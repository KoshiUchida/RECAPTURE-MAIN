/**
 * @file   AnimationStateBace.h
 *
 * @brief  アニメーション状態基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/12/10
 */

#pragma once
#ifndef ANIMATION_STATE_BASE
#define ANIMATION_STATE_BASE

#include <CCC/Animation/AnimationPlayer.h>

namespace CCC
{
	namespace Resources
	{
		class ModelWithBoneResource;
	}

	namespace Animations
	{
		/// <summary>
		/// アニメーション状態基底クラス
		/// </summary>
		class AnimationStateBace
		{
		public:
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="modelName">アニメーションを適用するモデル</param>
			/// <param name="firstName">最初のアニメーション名</param>
			AnimationStateBace(const std::string& modelName, const std::string& firstName);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~AnimationStateBace();

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime);

			/// <summary>
			/// 描画処理
			/// </summary>
			/// <param name="world">ワールド座標</param>
			/// <param name="view">ビュー行列</param>
			void Render(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view);

			/// <summary>
			/// 終了処理
			/// </summary>
			void Finalize();

			/// <summary>
			/// アニメーション変更要求
			/// </summary>
			/// <param name="animationName">変更先のアニメーション名</param>
			/// <param name="blendTime">変化するまでの時間</param>
			void RequestAnimationChange(
				const std::string& animationName,
				float blendTime);

			/// <summary>
			/// アニメーションの追加
			/// </summary>
			/// <param name="animationName">追加するアニメーション名</param>
			void AddAnimationPlayer(const std::string& animationName);

			/// <summary>
			/// アニメーション時間の設定
			/// </summary>
			/// <param name="time"></param>
			void SetAnimationTime(double time) {
				m_AnimationPlayers.at(m_CurrentAnimationName)->SetTime(time);
			}

			/// <summary>
			/// アニメーションの再生速度の倍率設定
			/// </summary>
			/// <param name="speed">再生倍率</param>
			void SetAnimationSpeed(float speed) {
				m_AnimationSpeed = speed;
			}

			/// <summary>
			/// アニメーションの再生速度の倍率リセット
			/// </summary>
			void ResetAnimationSpeed() {
				m_AnimationSpeed = 1.0f;
			}

			/// <summary>
			/// アニメーションが終了したかどうかを返す
			/// </summary>
			bool EndCurrentAnimation() const {
				return m_AnimationPlayers.at(m_CurrentAnimationName)->IsEnd();
			}

			/// <summary>
			/// アニメーションの総再生時間を取得
			/// </summary>
			double GetCurrentAnimationDuration() const {
				return m_AnimationPlayers.at(m_CurrentAnimationName)->GetDuration();
			}

			/// <summary>
			/// 現在のアニメーション名を取得
			/// </summary>
			/// <returns></returns>
			std::string GetCurrentAnimationName() const {
				return m_CurrentAnimationName;
			}

		private:
			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
			
			// アニメーションプレイヤーの配列
			using AnimationPlayerCollection = std::unordered_map<std::string, std::unique_ptr<CCC::Animations::AnimationPlayer>>;
			AnimationPlayerCollection m_AnimationPlayers;

			// 現在のアニメーション名
			std::string m_CurrentAnimationName;

			// アニメーション速度倍率
			float m_AnimationSpeed;

			// ブレンドするアニメーション名
			std::string m_BlendToAnimationName;

			// ブレンドアニメーションの総時間
			float m_BlendTime;

			// ブレンドアニメーションの時間
			float m_CurrentBlendTime;

			// アニメーションを適用するモデル
			CCC::Resources::ModelWithBoneResource* mp_Model;
		};
	}
}

#endif // !ANIMATION_STATE_BASE

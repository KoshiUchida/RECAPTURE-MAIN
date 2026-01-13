/**
 * @file   AnimationPlayer.h
 *
 * @brief  アニメーションプレイヤーのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/12/10
 */

#pragma once

namespace CCC
{
	// 前方宣言
	namespace Resources
	{
		class AnimationResource;
		class ModelWithBoneResource;
	}

	namespace Animations
	{
		class AnimationPlayer
		{
		public:

			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="resourceName">再生するアニメーションリソースの名前</param>
			AnimationPlayer(const std::string& resourceName);

			/// <summary>
			/// デストラクタ
			/// </summary>
			~AnimationPlayer();

			/// <summary>
			/// データの解放
			/// </summary>
			void Release();

			/// <summary>
			/// バインド
			/// </summary>
			/// <param name="model">バインド先のモデル</param>
			bool Bind(CCC::Resources::ModelWithBoneResource* p_Model);

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="delta">経過時間</param>
			void Update(float delta);

			/// <summary>
			/// 適用処理
			/// </summary>
			/// <param name="p_Model">モデルへのポインタ</param>
			void Apply(CCC::Resources::ModelWithBoneResource* p_Model) const;

			/// <summary>
			/// アニメーション時間のリセット
			/// </summary>
			void ResetTime() {
				m_Time = 0.0;
			}

			/// <summary>
			/// 処理が終了しているかどうかを返すメンバ関数。
			/// </summary>
			/// <returns>処理が終了している場合は true を、そうでない場合は false を返します。</returns>
			bool IsEnd() const {
				return m_IsEnd;
			}

			/// <summary>
			/// 時間の設定
			/// </summary>
			void SetTime(double time) {
				m_Time = time;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			double GetDuration() const {
				return m_Duration;
			}

			/// <summary>
			/// ローカルボーン行列だけを取得
			/// </summary>
			void GetLocalBoneTransforms(
				const DirectX::Model& model,
				size_t nbones,
				DirectX::XMMATRIX* localBones) const;

			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// アニメーションデータ
			CCC::Resources::AnimationResource* mp_AnimationResource;
			// アニメーション時間
			double                              m_Time;
			// アニメーションボーン変換行列
			DirectX::ModelBone::TransformArray  m_Bones;
			// アニメーションの再生間隔
			double                              m_Duration;
			// 現在のアニメーション時間
			double                              m_PrevAnimTime;
			// アニメーション経過フラグ
			bool                                m_IsEnd;
		};



		/// <summary>
		/// アニメーションのブレンド適用
		/// </summary>
		/// <param name="model">適用先</param>
		/// <param name="nbones">ボーンのサイズ</param>
		/// <param name="a">ブレンドアニメーション１</param>
		/// <param name="b">ブレンドアニメーション２</param>
		/// <param name="alpha">ブレンドの配分</param>
		/// <param name="boneTransforms">ボーン</param>
		void ApplyBlendSDKMESH(
			const DirectX::Model* model,
			size_t nbones,
			const AnimationPlayer* a,
			const AnimationPlayer* b,
			float alpha,
			DirectX::XMMATRIX* boneTransforms);
	}
}

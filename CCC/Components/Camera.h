/**
 * @file   Camera.h
 *
 * @brief  カメラコンポネートクラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2025/12/24
 *
 * カメラ機能を提供するコンポーネントクラス
 */

#pragma once

 // 基底クラス
#include <CCC/Components/UpdataComponentBase.h>

namespace CCC
{
	namespace Components
	{
		/// <summary>
		/// カメラコンポーネントクラス
		/// </summary>
		class Camera :
			public CCC::Bases::UpdataComponentBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="p_Owner">保持するオブジェクトへのポインタ</param>
			Camera(CCC::Interfaces::IObject* p_Owner, float eyeSpeedRate = 1.0f, float targetSpeedRate = 1.0f) noexcept;
			
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~Camera() noexcept;

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime) override;



			// ---------------------------------------------------------------------- //
			// 設定関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// カメラの視点注点の設定
			/// </summary>
			/// <param name="eye">視点座標</param>
			/// <param name="target">注点座標</param>
			void SetCameraPositions(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

			/// <summary>
			/// カメラの位置への距離の差分に対する割合を設定
			/// </summary>
			void SetCameraEyeMoveRate(float rate) { m_EyeMoveRate = rate; }

			/// <summary>
			/// 注視点への距離の差分に対する割合を設定
			/// </summary>
			void SetCameraTargetMoveRate(float rate) { m_TargetMoveRate = rate; }
			


			// ---------------------------------------------------------------------- //
			// 取得関数
			// ---------------------------------------------------------------------- //

			/// <summary>
			/// 視点の位置を取得
			/// </summary>
			const DirectX::SimpleMath::Vector3& GetEyePosition() { return m_EyePosition; }

			/// <summary>
			/// 注目点の位置を取得
			/// </summary>
			const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_TargetPosition; }

			/// <summary>
			/// ビュー行列の取得
			/// </summary>
			const DirectX::SimpleMath::Matrix& GetView() const { return m_View; }



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:

			// 視点の位置
			DirectX::SimpleMath::Vector3 m_EyePosition;

			// 注目点の位置
			DirectX::SimpleMath::Vector3 m_TargetPosition;

			// ビュー行列
			DirectX::SimpleMath::Matrix m_View;

			// カメラの位置への距離の差分に対する割合
			float m_EyeMoveRate;

			// 注視点への距離の差分に対する割合
			float m_TargetMoveRate;

			// カメラの位置の初期化フラグ
			bool m_InitFlag;
		};
	}
}

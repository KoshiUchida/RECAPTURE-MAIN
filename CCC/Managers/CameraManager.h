/**
 * @file   CameraManager.h
 *
 * @brief  カメラの管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/07
 * ビュー行列の取得をシンプルにするための管理クラス
 * 改修の余地あり
 * 
 * 2026/01/07
 * ビュー行列からカメラの方向を取得する関数の実装
 */

// 多重インクルードガード
#pragma once

namespace CCC
{
	namespace Managers
	{
		/// <summary>
		/// カメラ管理クラス
		/// </summary>
		class CameraManager final
		{
			// ---------------------------------------------------------------------- //
			// シングルトンパターン関連
			// ---------------------------------------------------------------------- //
		private:
			// 実体を他のクラスが作れないように

			/// <summary>
			/// コンストラクタ
			/// </summary>
			CameraManager();

			// このクラスの実体
			static std::unique_ptr<CameraManager> s_Instance;


		public:
			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns>リソース管理クラスへのポインタ</returns>
			static CameraManager* GetInstance();



			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			~CameraManager();

			/// <summary>
			/// ビュー行列の設定
			/// </summary>
			void SetView(const DirectX::SimpleMath::Matrix& view) {
				m_View = view;
			}

			/// <summary>
			/// ビュー行列の取得
			/// </summary>
			DirectX::SimpleMath::Matrix GetView() const {
				return m_View;
			}

			/// <summary>
			/// カメラの前ベクトルの取得
			/// </summary>
			DirectX::SimpleMath::Vector3 GetForwardXZ() const;

			/// <summary>
			/// カメラの右ベクトルの取得
			/// </summary>
			/// <returns></returns>
			DirectX::SimpleMath::Vector3 GetRightXZ() const;



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// ビュー行列
			DirectX::SimpleMath::Matrix m_View;
		};
	}
}

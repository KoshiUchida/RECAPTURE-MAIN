#pragma once

// 基底クラス
#include <CCC/Components/UpdataComponentBase.h>

#include "PawnBase.h"

#include "TeamID.h"

// 前方宣言
class PawnManager;

namespace CCC
{
	namespace Components
	{
		class PawnCollider final:
			public CCC::Bases::UpdataComponentBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="p_Owner">保持するPawn基底オブジェクトクラスへのポインタ</param>
			/// <param name="colliderRadius">コライダーの半径</param>
			PawnCollider(CCC::Bases::PawnBase* p_Owner, float colliderRadius, PawnManager* p_Manager);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~PawnCollider();

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime) override;

			/// <summary>
			/// 外力の取得
			/// 取得時に自身の外力をリセットする
			/// </summary>
			DirectX::SimpleMath::Vector3 GetOutsideForce();



			// ---------------------------------------------------------------------- //
			// 設定関数
			// ---------------------------------------------------------------------- //
		public:
			void SetOutsideForce(const DirectX::SimpleMath::Vector3& force) {
				m_OutsideForce = force;
			}

			void SetActive(bool active) {
				m_Active = active;
			}


			// ---------------------------------------------------------------------- //
			// 取得関数
			// ---------------------------------------------------------------------- //
		public:
			DirectX::SimpleMath::Vector2 GetPosition() const {
				return m_Position;
			}

			float GetRadius() const {
				return m_Radius;
			}

			TeamID GetTeamID() const {
				return m_TeamID;
			}

			DirectX::SimpleMath::Vector3 GetVelocity() const {
				return m_Velocity;
			}

			bool IsSetOutsideForce() const {
				return m_Active && m_CoolTime > 0.0f && m_OutsideForce != DirectX::SimpleMath::Vector3::Zero;
			}

			bool GetActive() const {
				return m_Active;
			}


			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// コライダーのアクティブ
			bool m_Active;

			// 座標
			DirectX::SimpleMath::Vector2 m_Position;

			// あたり判定の半径
			float m_Radius;

			// チームID
			TeamID m_TeamID;

			// 自身のベロシティ
			DirectX::SimpleMath::Vector3 m_Velocity;

			// 与えられた外力（0の場合は与えられていない）
			DirectX::SimpleMath::Vector3 m_OutsideForce;

			// クールタイム
			float m_CoolTime;
		};
	}
}

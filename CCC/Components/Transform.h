/**
 * @file   Transform.h
 *
 * @brief  トランスフォームコンポネートクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/19
 *
 * 座標・回転・大きさの管理をするコンポネート 
 * 
 * 2025/12/19
 * m_Rotateの扱う回転をデグリ値からラジアン値に変更
 */

#pragma once
#ifndef TRANSFORM_DEFINE
#define TRANSFORM_DEFINE

#include <CCC/Components/ComponentBase.h>

namespace CCC
{
	namespace Components
	{
		class Transform :
			public CCC::Bases::ComponentBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="p_Owner">保持するオブジェクト</param>
			Transform(CCC::Interfaces::IObject* p_Owner) noexcept;

			/// <summary>
			/// デストラクタ
			/// </summary>
			~Transform() noexcept;

			/// <summary>
			/// ノーマライズ
			/// </summary>
			void Normalize();

			/// <summary>
			/// ワールド行列の取得
			/// </summary>
			DirectX::SimpleMath::Matrix GetWorldMatrix();



			// ---------------------------------------------------------------------- //
			// 設定関数
			// ---------------------------------------------------------------------- //
			void SetPosition(const DirectX::SimpleMath::Vector3   & position);
			void SetRotate  (const DirectX::SimpleMath::Vector3   & rotate);
			void SetRotate  (const DirectX::SimpleMath::Quaternion& quaternion);
			void SetScale   (const DirectX::SimpleMath::Vector3   & scale);

			void SetScale(float scale);

			void SetPositionX(float x);
			void SetPositionY(float y);
			void SetPositionZ(float z);

			void SetRotateX(float x);
			void SetRotateY(float y);
			void SetRotateZ(float z);

			void SetScaleX(float x);
			void SetScaleY(float y);
			void SetScaleZ(float z);



			// ---------------------------------------------------------------------- //
			// 取得関数
			// ---------------------------------------------------------------------- //
			DirectX::SimpleMath::Vector3 GetPosition() const;
			DirectX::SimpleMath::Vector3 GetRotate  () const;
			DirectX::SimpleMath::Vector3 GetScale   () const;

			DirectX::SimpleMath::Quaternion GetQuaternion();

			float GetPositionX() const;
			float GetPositionY() const;
			float GetPositionZ() const;

			float GetRotateX() const;
			float GetRotateY() const;
			float GetRotateZ() const;

			float GetScaleX() const;
			float GetScaleY() const;
			float GetScaleZ() const;



			// ---------------------------------------------------------------------- //
			// 加算関数
			// ---------------------------------------------------------------------- //
			void AddPosition(const DirectX::SimpleMath::Vector3& position);
			void AddRotate  (const DirectX::SimpleMath::Vector3& rotate);

			void AddPositionX(float x);
			void AddPositionY(float y);
			void AddPositionZ(float z);

			void AddRotateX(float x);
			void AddRotateY(float y);
			void AddRotateZ(float z);

			void AddScaleX(float x);
			void AddScaleY(float y);
			void AddScaleZ(float z);

		private:
			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
			
			// 座標
			DirectX::SimpleMath::Vector3 m_Position;

			// 向き（ラジアン値）
			DirectX::SimpleMath::Vector3 m_Rotate;

			// 大きさ
			DirectX::SimpleMath::Vector3 m_Scale;
		};
	}
}

#endif // !TRANSFORM_DEFINE

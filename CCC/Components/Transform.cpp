/**
 * @file   Transform.cpp
 *
 * @brief  トランスフォームコンポネートクラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/02/04
 *
 * 座標・回転・大きさの管理をするコンポネート
 * 
 * 2025/12/19
 * ワールド座標の取得関数に
 * 回転を正規化する処理を追加
 * 
 * 2026/02/04
 * メンバ変数m_Scaleが生成時に1倍になっていなかったため、コンストラクタで生成時の初期化処理を追加
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "Transform.h"

namespace CCC::Components
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	Transform::Transform(CCC::Interfaces::IObject* p_Owner) noexcept :
		ComponentBase(p_Owner),
		m_Position(DirectX::SimpleMath::Vector3::Zero),
		m_Rotate  (DirectX::SimpleMath::Vector3::Zero),
		m_Scale   (DirectX::SimpleMath::Vector3::One)
	{
	}

	Transform::~Transform() noexcept = default;

	static float WrapPi(float a)
	{
		a = std::fmod(a + DirectX::XM_PI, DirectX::XM_2PI);
		if (a < 0) a += DirectX::XM_2PI;
		return a - DirectX::XM_PI;
	}

	void Transform::Normalize()
	{
		m_Rotate.x = WrapPi(m_Rotate.x);
		m_Rotate.y = WrapPi(m_Rotate.y);
		m_Rotate.z = WrapPi(m_Rotate.z);
	}

	DirectX::SimpleMath::Matrix Transform::GetWorldMatrix()
	{
		using namespace DirectX::SimpleMath;

		// ワールド行列
		Matrix world = Matrix::Identity;

		// 平行移動する行列を作成する
		Matrix trans = Matrix::CreateTranslation(GetPosition());

		// 正規化
		Normalize();

		// クォータニオンから回転する行列を作成する
		Matrix rot = Matrix::CreateFromQuaternion(GetQuaternion());

		// 拡大する行列を作成する
		Matrix scale = Matrix::CreateScale(GetScale());

		// ワールド行列へ統合
		world = scale * rot * trans;

		return world;
	}



	// ---------------------------------------------------------------------- //
	// 設定関数
	// ---------------------------------------------------------------------- //

	void Transform::SetPosition(const DirectX::SimpleMath::Vector3& position)
	{
		m_Position = position;
	}

	void Transform::SetRotate(const DirectX::SimpleMath::Vector3& rotate)
	{
		m_Rotate = rotate;
	}

	void Transform::SetRotate(const DirectX::SimpleMath::Quaternion& quaternion)
	{
		m_Rotate = quaternion.ToEuler();
	}

	void Transform::SetScale(const DirectX::SimpleMath::Vector3& scale)
	{
		m_Scale = scale;
	}

	void Transform::SetScale(float scale)
	{
		m_Scale = DirectX::SimpleMath::Vector3(scale, scale, scale);
	}

	void Transform::SetPositionX(float x)
	{
		m_Position.x = x;
	}

	void Transform::SetPositionY(float y)
	{
		m_Position.y = y;
	}

	void Transform::SetPositionZ(float z)
	{
		m_Position.z = z;
	}

	void Transform::SetRotateX(float x)
	{
		m_Rotate.x = x;
	}

	void Transform::SetRotateY(float y)
	{
		m_Rotate.y = y;
	}

	void Transform::SetRotateZ(float z)
	{
		m_Rotate.z = z;
	}

	void Transform::SetScaleX(float x)
	{
		m_Scale.x = x;
	}

	void Transform::SetScaleY(float y)
	{
		m_Scale.y = y;
	}

	void Transform::SetScaleZ(float z)
	{
		m_Scale.z = z;
	}



	// ---------------------------------------------------------------------- //
	// 取得関数
	// ---------------------------------------------------------------------- //

	DirectX::SimpleMath::Vector3 Transform::GetPosition() const
	{
		return m_Position;
	}

	DirectX::SimpleMath::Vector3 Transform::GetRotate() const
	{
		return m_Rotate;
	}

	DirectX::SimpleMath::Quaternion Transform::GetQuaternion()
	{
		return DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
			DirectX::SimpleMath::Vector3(m_Rotate.y, m_Rotate.x, m_Rotate.z)
		);
	}

	DirectX::SimpleMath::Vector3 Transform::GetScale() const
	{
		return m_Scale;
	}

	float Transform::GetPositionX() const
	{
		return m_Position.x;
	}

	float Transform::GetPositionY() const
	{
		return m_Position.y;
	}

	float Transform::GetPositionZ() const
	{
		return m_Position.z;
	}

	float Transform::GetRotateX() const
	{
		return m_Rotate.x;
	}

	float Transform::GetRotateY() const
	{
		return m_Rotate.y;
	}

	float Transform::GetRotateZ() const
	{
		return m_Rotate.z;
	}

	float Transform::GetScaleX() const
	{
		return m_Scale.x;
	}

	float Transform::GetScaleY() const
	{
		return m_Scale.y;
	}

	float Transform::GetScaleZ() const
	{
		return m_Scale.z;
	}



	// ---------------------------------------------------------------------- //
	// 加算関数
	// ---------------------------------------------------------------------- //

	void Transform::AddPosition(const DirectX::SimpleMath::Vector3& position)
	{
		m_Position += position;
	}

	void Transform::AddRotate(const DirectX::SimpleMath::Vector3& rotate)
	{
		m_Rotate += rotate;
	}

	void Transform::AddPositionX(float x)
	{
		m_Position.x += x;
	}

	void Transform::AddPositionY(float y)
	{
		m_Position.y += y;
	}

	void Transform::AddPositionZ(float z)
	{
		m_Position.z += z;
	}

	void Transform::AddRotateX(float x)
	{
		m_Rotate.x += x;
	}

	void Transform::AddRotateY(float y)
	{
		m_Rotate.y += y;
	}

	void Transform::AddRotateZ(float z)
	{
		m_Rotate.z += z;
	}

	void Transform::AddScaleX(float x)
	{
		m_Scale.x += x;
	}

	void Transform::AddScaleY(float y)
	{
		m_Scale.y += y;
	}

	void Transform::AddScaleZ(float z)
	{
		m_Scale.z += z;
	}
}

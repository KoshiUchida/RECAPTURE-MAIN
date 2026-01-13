/**
 * @file   CameraManager.cpp
 *
 * @brief  カメラの管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/07
 * ビュー行列の取得をシンプルにするための管理クラス
 * 
 * 2026/01/07
 * ビュー行列の逆行列を取得
 */

#include "pch.h"
#include "CameraManager.h"

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<CameraManager> CameraManager::s_Instance = nullptr;

	CameraManager::CameraManager()
	{
		m_View = DirectX::SimpleMath::Matrix::Identity;
	}

	CameraManager* CameraManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new CameraManager());

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	CameraManager::~CameraManager() = default;


	DirectX::SimpleMath::Vector3 CameraManager::GetForwardXZ() const
	{
		// Viewの逆行列＝カメラのワールド変換
		DirectX::SimpleMath::Matrix invView = m_View.Invert();

		// Forwardをワールドで取得
		DirectX::SimpleMath::Vector3 f = invView.Forward();

		// 水平化（XZ成分）
		f.y = 0.0f;
		if (f.LengthSquared() > 0.0001f) f.Normalize();

		return f;
	}

	DirectX::SimpleMath::Vector3 CameraManager::GetRightXZ() const
	{
		DirectX::SimpleMath::Matrix invView = m_View.Invert();
		DirectX::SimpleMath::Vector3 r = invView.Right();

		r.y = 0.0f;
		if (r.LengthSquared() > 0.0001f) r.Normalize();

		return r;
	}
}

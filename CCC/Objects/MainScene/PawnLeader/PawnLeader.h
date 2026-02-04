/**
 * @file   PawnLeader.h
 *
 * @brief  ポーンリーダーオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/28
 * 
 * 部隊を指揮するポーンリーダーオブジェクトクラス
 * 
 * 2026/01/05
 * 隊員の陣形所定位置差分の平均値を保持
 * 陣形密度の安定度が取得できる関数を実装
 * 
 * 
 * 2026/01/06
 * 安定度を評価するためのEnumクラスを追加
 * 正方形に陣形を設定する関数の追加
 * 
 * 2026/01/07
 * スキルの状態を実装
 * 
 * 2026/01/28
 * ファランクス陣形の追加
 * 上記の陣形スキルが発動中かのフラッグをメンバ変数に追加
 */

// 多重インクルードガード
#pragma once

 // 基底クラス
#include <CCC/Objects/PawnBase.h>

 // 前方宣言
namespace CCC::Managers
{
	class InputManager;
	class CameraManager;
}
class PawnManager;

/// <summary>
/// ポーンリーダーオブジェクトクラス
/// </summary>
class PawnLeader :
	public CCC::Bases::PawnBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PawnLeader(PawnManager* p_PawnManager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PawnLeader();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override final;

	/// <summary>
	/// この派生クラスの更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Process(float elapsedTime) override final;

	/// <summary>
	/// 座標の設定
	/// </summary>
	void SetPosition(const DirectX::SimpleMath::Vector3& position) override final;

	/// <summary>
	/// カメラのトランスフォームへのポインタを設定する
	/// </summary>
	void SetCameraTransform(CCC::Bases::ObjectBase* p_Camera);

	/// <summary>
	/// 陣形安定度の取得
	/// </summary>
	float GetFormationStability() const;


	// ---------------------------------------------------------------------- //
	// メンバ変数
	// ---------------------------------------------------------------------- //
private:
	// 入力管理クラスへのポインタ
	CCC::Managers::InputManager* mp_InputManager;

	// カメラ管理クラスへのポインタ
	CCC::Managers::CameraManager* mp_CameraManager;

	// ポーンオブジェクト郡へのポインタ配列
	std::vector<CCC::Bases::PawnBase*> m_PawnPointers;

	// カメラのトランスフォーム
	CCC::Components::Transform* mp_CameraTransform;

	// 移動しているか
	bool m_IsMove;

	PawnManager* mp_PawnManager;



	// ---------------------------------------------------------------------- //
	// 陣形安定度関連
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// 陣形安定度の状態種類
	/// </summary>
	enum class StabilityStates
	{
		Stable,		// 安定している
		Warning,	// 崩壊を警告
		Broken,		// 崩壊した
		Death,		// この部隊は死んだ
		Count		// 状態の数
	};

private:
	// 隊員の陣形所定位置との差分の平均値
	float m_AverageUnitDiff;

	// 陣形安定度の状態
	StabilityStates m_StabilityState;

	// 崩壊しているか
	bool m_IsBroken;

	// 崩壊している秒数
	float m_BrokenTime;

	// 回復までの時間
	float m_RecoveryTime;

public:
	/// <summary>
	/// 隊員の陣形所定位置との差分を取得
	/// </summary>
	float GetAverageUnitDiff() const {
		return m_AverageUnitDiff;
	}

	/// <summary>
	/// 陣形安定度の状態取得
	/// </summary>
	StabilityStates GetStabilityState() const {
		return m_StabilityState;
	}

	/// <summary>
	/// 回復までの時間を取得
	/// </summary>
	float GetRecoveryTime() const {
		return m_RecoveryTime;
	}




	// ---------------------------------------------------------------------- //
	// 陣形の設定
	// ---------------------------------------------------------------------- //
private:

	/// <summary>
	/// 隊員の座標を目標の陣形所定位置に設定する
	/// </summary>
	void PawnsPositionReset();

	/// <summary>
	/// 方陣
	/// </summary>
	/// <param name="between">隊員間の距離</param>
	void FormationSquare(float between);

	/// <summary>
	/// くさび形陣形
	/// </summary>
	/// <param name="between">隊員間の距離</param>
	void FormationWedge(float between);

	/// <summary>
	/// ファランクス
	/// </summary>
	/// <param name="between">隊員間の距離</param>
	void FormationPhalanx(float between);

private:
	// ファランクスか？
	bool m_ActiveSkillPhalanx;




	// ---------------------------------------------------------------------- //
	// 陣形スキル関連
	// ---------------------------------------------------------------------- //
public:

	/// <summary>
	/// スキルゲージの状態の種類
	/// </summary>
	enum class SkillStates
	{
		Inactive,	// 発動可能
		Active,		// 発動中
		Recovering,	// 回復中
		Count,		// 状態の種類の数
	};

private:
	// スキルゲージの状態
	SkillStates m_SkillState;

	// スキルゲージ
	float m_SkillGauge;

	// スキル発動時の向き
	DirectX::SimpleMath::Vector3 m_SkillAngle;


public:
	/// <summary>
	/// スキルの状態を取得
	/// </summary>
	SkillStates GetSkillState() const {
		return m_SkillState;
	}

	/// <summary>
	/// スキルゲージのパーセンテージを取得
	/// </summary>
	float GetSkillGauge() const {
		return m_SkillGauge;
	}
};

/**
 * @file   EnemyPawnLeader.h
 *
 * @brief  敵ポーンリーダーオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/13
 *
 * 敵部隊を指揮する敵ポーンリーダーオブジェクトクラス
 *
 * 2026/01/13
 * 作成
 *
 */

#pragma once

// 基底クラス
#include <CCC/Objects/PawnBase.h>

class PawnManager;

class EnemyPawnLeader :
	public CCC::Bases::PawnBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyPawnLeader(const DirectX::SimpleMath::Vector3& spawnPosition, PawnManager* p_Manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyPawnLeader();

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
	/// 陣形安定度の取得
	/// </summary>
	float GetFormationStability() const;



	// ---------------------------------------------------------------------- //
	// メンバ変数
	// ---------------------------------------------------------------------- //
private:
	// ポーンオブジェクト郡へのポインタ配列
	std::vector<CCC::Bases::PawnBase*> m_PawnPointers;

	// 攻撃の状態
	int m_Attack;

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


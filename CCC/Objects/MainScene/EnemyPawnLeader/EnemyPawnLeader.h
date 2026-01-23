/**
 * @file   EnemyPawnLeader.h
 *
 * @brief  敵ポーンリーダーオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 *
 * 敵部隊を指揮する敵ポーンリーダーオブジェクトクラス
 *
 * 2026/01/13
 * 作成
 * 
 * 2026/01/22
 * ターゲットを設定できるようになった
 * ターゲットはビヘイビアツリーに使用
 * 
 * 2026/01/23
 * 固有名詞が設定できるようになった
 */

// 多重インクルードガード
#pragma once

// 基底クラス
#include <CCC/Objects/PawnBase.h>

// ステートマシン用の状態数列
#include "EnemyPawnLeaderStates.h"

// 前方宣言
class PawnManager;

// メッセンジャーで扱う構造体
struct AddressedPayload
{
	CCC::Interfaces::IObject* p_Addressed;	// 宛名
	EnemyPawnLeaderStates     p_Payload;	// 郵便物
};

/// <summary>
/// 敵ポーン隊長オブジェクトクラス
/// </summary>
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
	/// <param name="spawnPosition">スポーン地点</param>
	/// <param name="p_Manager">ポーン管理オブジェクトクラスへのポインタ</param>
	/// <param name="p_Target">ターゲットへのポインタ</param>
	/// <param name="name">このエネミーの固有名詞</param>
	EnemyPawnLeader(
		const DirectX::SimpleMath::Vector3& spawnPosition,
		PawnManager*                        p_Manager,
		CCC::Bases::PawnBase*               p_Target,
		const std::string&                  name
	);

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

	/// <summary>
	/// ターゲットオブジェクトの取得
	/// </summary>
	CCC::Bases::PawnBase* GetTarget();



	// ---------------------------------------------------------------------- //
	// メンバ変数
	// ---------------------------------------------------------------------- //
private:
	// ポーン管理オブジェクトクラスへのポインタ
	PawnManager* mp_PawnManager;

	// ポーンオブジェクト郡へのポインタ配列
	std::vector<CCC::Bases::PawnBase*> m_PawnPointers;

	// ターゲットのオブジェクへのポインタ
	CCC::Bases::PawnBase* mp_Target;

	// 現在の状態
	EnemyPawnLeaderStates m_State;

	// 名前
	std::string m_Name;

	// 移動しているか
	bool m_IsMove;



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

	/// <summary>
	/// スキルの状態へのポインタを取得
	/// </summary>
	EnemyPawnLeader::SkillStates* GetSkillStatePointer() {
		return &m_SkillState;
	}
};


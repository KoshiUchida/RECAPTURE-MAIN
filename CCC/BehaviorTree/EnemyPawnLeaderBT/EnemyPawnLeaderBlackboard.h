/**
 * @file   EnemyPawnLeaderBlackboard.h
 *
 * @brief  敵の隊長のブラックボードのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 * 敵Pawn隊長のビヘイビアツリー用の
 * ブラックボードが定義された構造体
 * 
 * 2026/01/22
 * 作成
 * 
 * 2026/01/23
 * 「攻撃」の判断をするための変数を追加
 */

// 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/IBlackboard.h>

// 関係のあるクラス
#include <CCC/Components/Transform.h>
#include <CCC/Objects/MainScene/EnemyPawnLeader/EnemyPawnLeaderStates.h>
#include <CCC/Objects/MainScene/EnemyPawnLeader/EnemyPawnLeader.h>

/// <summary>
/// 敵隊長のブラックボード
/// </summary>
struct EnemyPawnLeaderBlackboard :
	public CCC::Interfaces::IBlackboard
{
	// この指示を受けるオブジェクト
	CCC::Interfaces::IObject* p_Body = nullptr;

	EnemyPawnLeaderStates state = EnemyPawnLeaderStates::Wait;	// ステートマシンの状態

	CCC::Components::Transform* p_MyTransform     = nullptr;	// 自分のトランスフォームへのポインタ
	CCC::Components::Transform* p_TargetTransform = nullptr;	// ターゲットのトランスフォームへのポインタ

	float viewDistance = 30.0f;	// 視界
	float stopDistance = 1.0f;	// 止まる距離

	float distanceToTarget = 0.0f;	// ターゲットとの距離


	float attackDistance                        = 20.0f;	// 攻撃できる距離
	EnemyPawnLeader::SkillStates* p_SkillStates = nullptr;	// スキルの状態


	/// <summary>
	/// 与えられる2つの数字の中間に存在するか
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="value">調べる値</param>
	bool IsBetween(float min, float max, float value)
	{
		return min <= value && value <= max;
	}
};

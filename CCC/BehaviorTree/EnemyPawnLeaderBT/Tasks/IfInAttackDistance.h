/**
 * @file   IfInAttackDistance.h
 *
 * @brief  ターゲットとの距離が攻撃できる距離にあるか判定タスクのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 * ターゲットとの距離が攻撃できる距離にあるかを判定するタスクノード
 * あるならSUCCESSを返し、そうでないならFAILURE
 */

 // 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// ターゲットとの距離が攻撃できる距離にあるか判定タスクノードクラス
/// </summary>
class IfInAttackDistance final :
	public CCC::Interfaces::INode
{
public:
	/// <summary>
	/// ノードの処理
	/// </summary>
	/// <param name="blackboard">ブラックボード</param>
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard) override final
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);

		if (bb.IsBetween(0.0f, bb.attackDistance, bb.distanceToTarget))
			return CCC::Interfaces::NodeStatus::SUCCESS;

		return CCC::Interfaces::NodeStatus::FAILURE;
	}
};

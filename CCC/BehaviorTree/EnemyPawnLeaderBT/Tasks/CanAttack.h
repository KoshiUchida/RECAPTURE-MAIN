/**
 * @file   CanAttack.h
 *
 * @brief  攻撃ができるかの判定タスクのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 * 攻撃ができる状態にあるかを判定するタスクノード
 * スキルの状態がInactive(攻撃可能)なら
 * SUCCESSを返し、そうでないならFAILURE
 */

 // 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// 攻撃ができる状態にあるかを判定するタスクノードクラス
/// </summary>
class CanAttack final :
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

		if (*bb.p_SkillStates == EnemyPawnLeader::SkillStates::Inactive)
			return CCC::Interfaces::NodeStatus::SUCCESS;

		return CCC::Interfaces::NodeStatus::FAILURE;
	}
};

/**
 * @file   ChangToAttack.h
 *
 * @brief  「攻撃」状態にするタスクのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 * 「攻撃」状態にするタスクノードクラス
 */

 // 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// 「攻撃」状態に変更するタスクノードクラス
/// </summary>
class ChengeToAttack :
	public CCC::Interfaces::INode
{
	/// <summary>
	/// ノードの処理
	/// </summary>
	/// <param name="blackboard">ブラックボード</param>
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard)
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);

		bb.state = EnemyPawnLeaderStates::Attack;

		return CCC::Interfaces::NodeStatus::SUCCESS;
	}
};


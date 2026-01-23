/**
 * @file   ChangeToWait.h
 *
 * @brief  「待機」状態にするタスクのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/22
 * 「待機」状態にするタスクノードクラス
 */

// 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// 「待機」状態に変更するタスクノードクラス
/// </summary>
class ChengeToWait :
	public CCC::Interfaces::INode
{
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard)
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);

		bb.state = EnemyPawnLeaderStates::Wait;

		return CCC::Interfaces::NodeStatus::SUCCESS;
	}
};


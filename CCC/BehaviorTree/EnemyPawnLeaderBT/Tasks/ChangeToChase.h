/**
 * @file   ChangeToChase.h
 *
 * @brief  「追跡」状態にするタスクのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/22
 * 「追跡」状態にするタスクノードクラス
 */

// 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// 「追跡」状態に変更するタスクノードクラス
/// </summary>
class ChengeToChase final:
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

		bb.state = EnemyPawnLeaderStates::Chase;

		return CCC::Interfaces::NodeStatus::SUCCESS;
	}
};


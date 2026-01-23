/**
 * @file   IfOutDistance.h
 *
 * @brief  ターゲットが追う距離にあるかを判定するデコレータのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/22
 * 「追跡」状態にするタスクノードクラス
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include <CCC/BehaviorTree/Bases/DecoratorNodeBase.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// ターゲットが追う距離にあるかを判定するデコレータノードクラス
/// </summary>
class IfInDistance :
	public CCC::Bases::DecoratorNodeBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="child">子ノード</param>
	IfInDistance(std::unique_ptr<CCC::Interfaces::INode> child) :
		DecoratorNodeBase(std::move(child))
	{
	}

	/// <summary>
	/// ノードの処理
	/// </summary>
	/// <param name="blackboard">ブラックボード</param>
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard) override
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);

		// 追う距離あるか
		if (bb.IsBetween(bb.stopDistance, bb.viewDistance, bb.distanceToTarget))
		{
			return GetChild()->Tick(bb);
		}

		// ない場合FAILUREを返す
		return CCC::Interfaces::NodeStatus::FAILURE;
	}
};

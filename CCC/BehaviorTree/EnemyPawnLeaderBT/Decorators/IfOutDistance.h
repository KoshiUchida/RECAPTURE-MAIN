/**
 * @file   IfOutDistance.h
 *
 * @brief  ターゲットが追う距離にないかを判定するデコレータのヘッダファイル
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
/// ターゲットが追う距離にないかを判定するデコレータノードクラス
/// </summary>
class IfOutDistance :
	public CCC::Bases::DecoratorNodeBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="child">子ノード</param>
	IfOutDistance(std::unique_ptr<CCC::Interfaces::INode> child) :
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

		// 追う距離にないか
		if (!bb.IsBetween(bb.stopDistance, bb.viewDistance, bb.distanceToTarget))
		{
			// 子ノードのTick関数を呼び出す。
			return GetChild()->Tick(bb);
		}

		// ある場合FAILUREを返す
		return CCC::Interfaces::NodeStatus::FAILURE;
	}
};

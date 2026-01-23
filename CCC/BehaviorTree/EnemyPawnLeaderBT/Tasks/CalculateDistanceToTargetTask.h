/**
 * @file   CalculateDistanceToTargetTask.h
 *
 * @brief  ターゲットとの２点間を計算するタスクノードクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/22
 * ターゲットとの２点間を計算するタスクノードクラス
 */

// 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

// DirectXTK
#include <SimpleMath.h>

/// <summary>
/// ターゲットとの2点間の距離を計算する
/// </summary>
class CalculateDistanceToTargetTask :
	public CCC::Interfaces::INode
{
public:
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard) override
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);


		// 2点の座標を取得
		DirectX::SimpleMath::Vector3 myPos     = bb.p_MyTransform->GetPosition();
		DirectX::SimpleMath::Vector3 targetPos = bb.p_TargetTransform->GetPosition();

		// 計算
		bb.distanceToTarget = (targetPos - myPos).Length();


		return CCC::Interfaces::NodeStatus::SUCCESS;
	}
};

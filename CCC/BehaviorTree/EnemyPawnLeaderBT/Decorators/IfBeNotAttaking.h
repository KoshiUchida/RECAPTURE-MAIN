/**
 * @file   IfBeNotAttacking.h
 *
 * @brief  攻撃中でないかの判定デコレータのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 * 攻撃中でないかの判定デコレータノードクラス
 * スキルの状態がInactive(攻撃可能)なら
 * FAILUREを返し、そうでないならSUCCESS
 */

 // 多重インクルードガード
#pragma once

// 実装元
#include <CCC/BehaviorTree/Bases/DecoratorNodeBase.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

/// <summary>
/// 攻撃中でないかの判定デコレータノードクラス
/// </summary>
class IfBeNotAttaking final :
	public CCC::Bases::DecoratorNodeBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="child">子ノード</param>
	IfBeNotAttaking(std::unique_ptr<CCC::Interfaces::INode> child) :
		DecoratorNodeBase(std::move(child))
	{
	}

	/// <summary>
	/// ノードの処理
	/// </summary>
	/// <param name="blackboard">ブラックボード</param>
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard) override final
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);

		if (*bb.p_SkillStates != EnemyPawnLeader::SkillStates::Active)
			return GetChild()->Tick(bb);

		return CCC::Interfaces::NodeStatus::FAILURE;
	}
};

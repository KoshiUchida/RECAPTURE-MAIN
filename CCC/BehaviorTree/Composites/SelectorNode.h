/**
 * @file   SelectorNode.h
 *
 * @brief  セレクターコンポジットノードクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/21
 * ビヘイビアツリーのセレクターコンポジットノードクラス
 * 複数の子ノードを順番に実行し、１つでもSUCCESSがでたら
 * その時点でSUCCESSを返すコンポジットノード
 *
 * 2026/01/21
 * 作成
 */

 // 多重インクルードガード
#pragma once

// 継承元
#include "../Bases/CompositeNodeBase.h"

namespace CCC::BehaviorTree::Composites
{
	/// <summary>
	/// ビヘイビアツリーのセレクターノードクラス
	/// </summary>
	class SelectorNode final:
		public CCC::Bases::CompositeNodeBase
	{
	public:
		/// <summary>
		/// ノードの処理
		/// </summary>
		/// <param name="blackboard">ブラックボード</param>
		/// <returns>ノードの状態</returns>
		CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard) override
		{
			// 子配列に格納された子ノードを順番に実行
			for (std::unique_ptr<CCC::Interfaces::INode>& child : GetChildren())
			{
				// 子ノードのTick関数を実行し結果を取得
				CCC::Interfaces::NodeStatus status = child->Tick(blackboard);

				// 子ノードがSUCCESSを返した場合
				if (status == CCC::Interfaces::NodeStatus::SUCCESS)
					// SUCCESSを返し、残りの子ノードは実行した
					return CCC::Interfaces::NodeStatus::SUCCESS;
			}

			// すべての子ノードが失敗した場合はFAILUREを返す
			return CCC::Interfaces::NodeStatus::FAILURE;
		}
	};
}

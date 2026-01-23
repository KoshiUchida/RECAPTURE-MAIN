/**
 * @file   SequenceNode.h
 *
 * @brief  シーケンスコンポジットノードクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/21
 * ビヘイビアツリーのシーケンスコンポジットノードクラス
 * 複数の子ノードを順番に実行し、
 * 途中でFAILUREやRUNNINGがあればその結果を返し、
 * 全ての子ノードがSUCCESSでSUCCESSを返すコンポジットノード
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
    /// ビヘイビアツリーのシーケンスノードクラス
    /// </summary>
    class SequenceNode final:
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
                
                // 子ノードがSUCCESS以外を返した場合
                // FAILUREまたはRUNNINGを返し処理を中断する
                if (status != CCC::Interfaces::NodeStatus::SUCCESS)
                    return status;
            }

            // 全ての子ノードがSUCCESSを返した場合、SUCCESSを返す
            return CCC::Interfaces::NodeStatus::SUCCESS;
        }
    };
}

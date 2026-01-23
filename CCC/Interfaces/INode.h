/**
 * @file   INode.h
 *
 * @brief  ノードインターフェースのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/21
 * ビヘイビアツリーのノードのインターフェース
 * すべてのノードはこのインターフェースから実装しなければならない
 * 
 * 2026/01/21
 * 作成
 */

// 多重インクルードガード
#pragma once

// 関係のあるインターフェース
#include "IBlackboard.h"

namespace CCC::Interfaces
{
	/// <summary>
	/// ノードの状態
	/// </summary>
	enum class NodeStatus
	{
		SUCCESS,	// ノードの処理が正常に完了した
		FAILURE,	// ノードの処理が失敗した
		RUNNING,	// ノードがまだ処理中である。
	};

	/// <summary>
	/// ビヘイビアツリーのノードインターフェースクラス
	/// </summary>
	class INode
	{
	public:
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~INode() = default;

		/// <summary>
		/// ノードの処理
		/// </summary>
		/// <param name="blackboard">ブラックボード</param>
		/// <returns>ノードの状態</returns>
		virtual NodeStatus Tick(IBlackboard& blackboard) = 0;
	};
}

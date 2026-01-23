/**
 * @file   CompositeBase.h
 *
 * @brief  コンポジットノード基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/21
 * ビヘイビアツリーのコンポジットノード基底クラス
 * すべてのコンポジットノードはこのクラスを継承しなければならない
 * 複数の子ノードを制御する
 *
 * 2026/01/21
 * 作成
 */

// 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// C++標準ライブラリ
#include <memory>
#include <vector>


namespace CCC::Bases
{
	/// <summary>
	/// ビヘイビアツリーのコンポジットノード基底クラス
	/// </summary>
	class CompositeNodeBase :
		public CCC::Interfaces::INode
	{
	public:
		/// <summary>
		/// 子ノードの配列を取得
		/// </summary>
		std::vector<std::unique_ptr<CCC::Interfaces::INode>>& GetChildren() {
			return m_Children;
		}

		/// <summary>
		/// 子ノードを追加する
		/// </summary>
		void AddChild(std::unique_ptr<CCC::Interfaces::INode> node) {
			m_Children.push_back(std::move(node));
		}

	private:
		// 子ノード
		std::vector<std::unique_ptr<CCC::Interfaces::INode>> m_Children;
	};
}


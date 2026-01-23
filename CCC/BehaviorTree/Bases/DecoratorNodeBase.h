/**
 * @file   DecoratorNodeBase.h
 *
 * @brief  デコレータノード基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/21
 * ビヘイビアツリーのデコレータノード基底クラス
 * すべてのデコレータノードはこのクラスを継承しなければならない
 * 子ノードに追加の条件や修飾を与える役割を持つ
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


namespace CCC::Bases
{
	/// <summary>
	/// ビヘイビアツリーのデコレータノード基底クラス
	/// </summary>
	class DecoratorNodeBase :
		public CCC::Interfaces::INode
	{
	public:
		/// <summary>
		/// 子ノードの取得
		/// </summary>
		INode* GetChild() {
			return m_Child.get();
		}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="child">子ノードのユニークポインタ</param>
		DecoratorNodeBase(std::unique_ptr<INode> child) :
			m_Child(std::move(child))
		{
		}

	private:
		// 子ノード
		std::unique_ptr<INode> m_Child;
	};
}

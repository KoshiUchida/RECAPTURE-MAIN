/**
 * @file   IdleState.h
 *
 * @brief  ポーンオブジェクトクラスの待機ステートのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/28
 * コメントを追加
 * 入力管理クラスへのポインタのメンバ変数があったが不要になったため削除
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴う、アドレスの変更
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */ 

// 多重インクルードガード
#pragma once

// 基底クラス
#include "StateBase.h"

namespace CCC::States
{
	/// <summary>
	/// 待機するステート
	/// </summary>
	class IdleState final :
		public CCC::Bases::StateBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		IdleState(CCC::Bases::CharacterBase* p_Owner);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~IdleState();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="elapsedTime">経過処理</param>
		void Update(float elapsedTime) override final;



		// ---------------------------------------------------------------------- //
		// メンバ関数
		// ---------------------------------------------------------------------- //
	private:
		/// <summary>
		/// 状態を変えるかどうかを判断する
		/// </summary>
		void RequestStateTransition();
	};
}

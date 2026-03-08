/**
 * @file   StandUpState.h
 *
 * @brief  キャラクタークラスの「立ち上がる」ステートのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/17
 * 作成
 * 
 * 2026/02/27
 * キャラクタークラスの実装に伴い、アドレスを変更
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
	/// 「立ち上がる」状態
	/// </summary>
	class StandUpState final :
		public CCC::Bases::StateBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		StandUpState(CCC::Bases::CharacterBase* p_Owner);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~StandUpState();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="elapsedTime">経過時間</param>
		void Update(float elapsedTime) override final;
	};
}

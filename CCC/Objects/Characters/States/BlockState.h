/**
 * @file   BlockState.h
 *
 * @brief  キャラクターステートの「防御」状態のヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 * この状態で敵の攻撃を受けても多少のぶっ飛びは効かない
 * 
 * 2026/01/28
 * 作成
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴うアドレスの変更
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
	/// 「防御」状態
	/// </summary>
	class BlockState final :
		public CCC::Bases::StateBase
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		BlockState(CCC::Bases::CharacterBase* p_Owner);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~BlockState();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="elapsedTime">経過時間</param>
		void Update(float elapsedTime) override final;



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		// 外力を受け止めた回数
		int m_OutSideCount;
	};
}

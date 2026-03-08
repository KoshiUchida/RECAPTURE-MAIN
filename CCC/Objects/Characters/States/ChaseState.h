/**
 * @file   ChaseState.h
 *
 * @brief  キャラクターの「追跡」ステートのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 * mp_Targetにセットされているオブジェクトに対して移動するステート
 * RUN_RADIUSの範囲よりも遠い場合は走り出す
 * 陣形安定度でその地点に向かうためのベロシティの向きが変動する
 * 不安定であるほどランダムな方向を向くようになる
 * 
 * 2026/03/04
 * 作成
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

// 多重インクルードガード
#pragma once

// 基底クラス
#include "StateBase.h"

namespace CCC
{
	namespace States
	{
		/// <summary>
		/// 追跡するステート
		/// </summary>
		class ChaseState final :
			public CCC::Bases::StateBase
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			ChaseState(CCC::Bases::CharacterBase* p_Owner);

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~ChaseState();

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
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
}

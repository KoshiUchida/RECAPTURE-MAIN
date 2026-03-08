/**
 * @file   StateBase.h
 *
 * @brief  キャラクターのステート基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/02/27
 * 作成
 * 
 * 2026/03/08
 * 共通化により、名称の「Paladin」を削除、
 * キャラクタークラスの共通化により、PaladinCharacteクラスからCharacterBaseクラスに変更
 */

// 多重インクルードガード
#pragma once

// 実装元クラス
#include <CCC/Interfaces/IState.h>


namespace CCC::Bases
{
	// 前方宣言
	class CharacterBase;

	/// <summary>
	/// ステートの基底クラス
	/// </summary>
	class StateBase :
		public CCC::Interfaces::IState
	{
		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="p_Owner">オーナーへのポインタ</param>
		StateBase(CharacterBase* p_Owner);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~StateBase();

		/// <summary>
		/// オーナーへのポインタ取得
		/// </summary>
		CharacterBase* GetOwner();



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		CharacterBase* mp_Owner;
	};
}

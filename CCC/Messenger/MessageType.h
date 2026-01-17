/**
 * @file   MessengeType.h
 *
 * @brief  メッセージ種別Enumクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/16
 * メッセージを受信・送信するためのクラス
 *
 * 2026/01/16
 * 作成
 */

// 多重インクルード阻止
#pragma once

namespace CCC
{
	namespace Messenger
	{
		/// <summary>
		/// メッセージの種別が定義されたEnumクラス
		/// </summary>
		enum class MessageType
		{
			// ---------------------------------------------------------------------- //
			// 入力情報
			// ---------------------------------------------------------------------- //
			INPUT_ATTACK,	// 「攻撃」の入力
		};
	}
}

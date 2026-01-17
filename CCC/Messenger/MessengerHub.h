/**
 * @file   MessengerHub.h
 *
 * @brief  メッセンジャー中継地点クラスのヘッダファイル
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

// C++標準ライブラリ
#include <unordered_map>
#include <memory>
#include <functional>
#include <any>
#include <list>

// メッセンジャーの種別
#include "MessageType.h"

namespace CCC
{
	namespace Messenger
	{
		/// <summary>
		/// メッセンジャー中継地点クラス
		/// メッセンジャーにおける受信・送信を担うクラス
		/// シングルトン構造のため、GetInstanceでポインタを取得することを前提として作られている
		/// </summary>
		class MessengerHub final
		{
			// ---------------------------------------------------------------------- //
			// シングルトン構造
			// ---------------------------------------------------------------------- //
		private:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			MessengerHub();

			// このクラスの実体
			static std::unique_ptr<MessengerHub> s_Instance;


		public:
			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns>リソース管理クラスへのポインタ</returns>
			static MessengerHub* GetInstance();



			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			~MessengerHub();



			// ---------------------------------------------------------------------- //
			// メッセンジャー機能
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// 郵便物
			/// </summary>
			struct PayLoad
			{
				std::any item;
			};
		private:
			// メッセンジャーの送信先リスト
			using Messege = std::function<void(PayLoad)>;
			using MessageMap = std::unordered_map<CCC::Messenger::MessageType, std::list<Messege>>;
			MessageMap m_Subscribers;

		public:
			// 登録
			void Subscribe(const CCC::Messenger::MessageType& type, std::function<void(PayLoad)> function);

			// メッセンジャーの受信と処理
			void Receive(const CCC::Messenger::MessageType& type, const PayLoad& payload);
		};
	}
}

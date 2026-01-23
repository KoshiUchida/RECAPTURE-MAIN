/**
 * @file   MessengerHub.h
 *
 * @brief  メッセンジャー中継地点クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * メッセージを受信・送信するためのクラス
 *
 * 2026/01/16
 * 作成
 * 
 * 2026/01/18
 * 登録を解除する関数を追加
 */

// 多重インクルード阻止
#pragma once

// C++標準ライブラリ
#include <unordered_map>
#include <memory>
#include <functional>
#include <any>
#include <vector>

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
			using MessageMap = std::unordered_map<CCC::Messenger::MessageType, std::vector<Messege>>;
			std::unique_ptr<MessageMap> m_Subscribers;
			
		public:
			/// <summary>
			/// メッセージの登録
			/// </summary>
			/// <param name="type">メッセージの種別</param>
			/// <param name="function">メッセージに対する処理</param>
			void Subscribe(const CCC::Messenger::MessageType& type, std::function<void(PayLoad)> function);

			/// <summary>
			/// メッセンジャーの受信と処理
			/// </summary>
			/// <param name="type">メッセージの種別</param>
			/// <param name="payload">郵便物</param>
			void Receive(const CCC::Messenger::MessageType& type, const PayLoad& payload);

			/// <summary>
			/// メッセージの解除
			/// </summary>
			/// <param name="type">解除する種別</param>
			void Unsubscribe(const CCC::Messenger::MessageType& type);
		};
	}
}

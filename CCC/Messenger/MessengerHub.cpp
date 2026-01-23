/**
 * @file   MessengerHub.cpp
 *
 * @brief  メッセンジャー中継地点クラスのソースファイル
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
 * メッセージの種別がないという種別の場合に早期リターンするようにに改修
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "MessengerHub.h"

namespace CCC::Messenger
{
	// ---------------------------------------------------------------------- //
	// シングルトン構造
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<MessengerHub> MessengerHub::s_Instance = nullptr;

	MessengerHub::MessengerHub()
	{
		m_Subscribers = std::make_unique<MessageMap>();
	}

	MessengerHub* MessengerHub::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new MessengerHub());

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	MessengerHub::~MessengerHub()
	{
		m_Subscribers->clear();
		m_Subscribers.reset();
	}



	// ---------------------------------------------------------------------- //
	// メッセンジャー機能
	// ---------------------------------------------------------------------- //

	void MessengerHub::Subscribe(const CCC::Messenger::MessageType& type, std::function<void(PayLoad)> function)
	{
		// 破棄されていたら早期リターン
		if (!m_Subscribers) return;
		
		// 種別がない場合、登録はしない
		if (type == CCC::Messenger::MessageType::NONE) return;

		// マップのtype先に要素追加する
		(*m_Subscribers.get())[type].push_back(std::move(function));
	}

	void MessengerHub::Receive(const CCC::Messenger::MessageType& type, const PayLoad& payload)
	{
		// 破棄されていたら早期リターン
		if (!m_Subscribers) return;

		// そのメッセージの種別はあるかを調べる
		auto it = m_Subscribers->find(type);
		if (type == CCC::Messenger::MessageType::NONE || it == m_Subscribers->end()) return;

		const auto handlers = it->second;

		// あるなら送信
		for (auto& handler : handlers)
			handler(payload);
	}

	void MessengerHub::Unsubscribe(const CCC::Messenger::MessageType& type)
	{
		// 破棄されていたら早期リターン
		if (!m_Subscribers) return;

		// そのメッセージの種別はあるかを調べる
		auto it = m_Subscribers->find(type);
		if (type == CCC::Messenger::MessageType::NONE || it == m_Subscribers->end()) return;

		// そのメッセージの種別を一斉削除
		it->second.clear();
	}
}

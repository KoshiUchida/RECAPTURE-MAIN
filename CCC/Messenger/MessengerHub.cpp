/**
 * @file   MessengerHub.cpp
 *
 * @brief  メッセンジャー中継地点クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/16
 * メッセージを受信・送信するためのクラス
 *
 * 2026/01/16
 * 作成
 */

// プリコンパイル済みヘッダー
#include "pch.h"

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
		m_Subscribers.clear();
	}



	// ---------------------------------------------------------------------- //
	// メッセンジャー機能
	// ---------------------------------------------------------------------- //

	void MessengerHub::Subscribe(const CCC::Messenger::MessageType& type, std::function<void(PayLoad)> function)
	{
		// マップのtype先に要素追加する
		m_Subscribers[type].push_back(std::move(function));
	}

	void MessengerHub::Receive(const CCC::Messenger::MessageType& type, const PayLoad& payload)
	{
		// そのメッセージの種別はあるかを調べる
		auto it = m_Subscribers.find(type);
		if (it == m_Subscribers.end()) return;

		const auto handlers = it->second;

		// あるなら送信
		for (auto& handler : handlers)
			handler(payload);
	}
}

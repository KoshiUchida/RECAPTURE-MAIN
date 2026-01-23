/**
 * @file   RequestChangeStateTask.h
 *
 * @brief  状態変更を要求するタスクノードクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/23
 * ステートマシーンの状態変化をメッセンジャーを通してオブジェクトに
 * 送信するタスクノードクラス
 *
 * 2026/01/22
 * 作成
 * 
 * 2026/01/23
 * メッセンジャーで送るメッセージに宛名を追加
 */

// 多重インクルードガード
#pragma once

// 実装元
#include <CCC/Interfaces/INode.h>

// ブラックボード
#include "../EnemyPawnLeaderBlackboard.h"

// 管理クラス
#include <CCC/Messenger/MessengerHub.h>
#include <CCC/Messenger/MessageType.h>

/// <summary>
/// 状態変更を要求するタスクノードクラス 
/// </summary>
class RequestChangeStateTask :
	public CCC::Interfaces::INode
{
public:
	CCC::Interfaces::NodeStatus Tick(CCC::Interfaces::IBlackboard& blackboard) override
	{
		EnemyPawnLeaderBlackboard& bb = static_cast<EnemyPawnLeaderBlackboard&>(blackboard);

		// メッセンジャーの取得
		CCC::Messenger::MessengerHub* p_mh = CCC::Messenger::MessengerHub::GetInstance();

		// 状態の変更を送信
		AddressedPayload payload{ bb.p_Body, bb.state };
		p_mh->Receive(CCC::Messenger::MessageType::RequestToEnemyPawnLeader_State, CCC::Messenger::MessengerHub::PayLoad(payload));


		return CCC::Interfaces::NodeStatus::SUCCESS;
	}
};

/**
 * @file   EnemyPawnLeaderBT.h
 *
 * @brief  敵ポーン隊長のビヘイビアツリーのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/22
 * 敵ポーン隊長のビヘイビアツリーオブジェクトクラス
 * 最初に実装されたのビヘイビアツリー
 *
 * 2026/01/22
 * 作成
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include <CCC/Objects/ObjectBase.h>

// ビヘイビアツリー
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/EnemyPawnLeaderBlackboard.h>
#include <CCC/Interfaces/INode.h>

// 前方宣言
class EnemyPawnLeader;

/// <summary>
/// 敵ポーン隊長のビヘイビアツリーオブジェクトクラス
/// </summary>
class EnemyPawnLeaderBT :
	public CCC::Bases::ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="object">このAIを扱うオブジェクト</param>
	EnemyPawnLeaderBT(EnemyPawnLeader* object);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyPawnLeaderBT();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

private:
	// ブラックボード
	EnemyPawnLeaderBlackboard m_Blackboard;

	// ルートノード
	std::unique_ptr<CCC::Interfaces::INode> m_Root;
};


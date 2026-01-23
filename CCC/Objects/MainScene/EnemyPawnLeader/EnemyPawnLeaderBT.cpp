/**
 * @file   EnemyPawnLeaderBT.cpp
 *
 * @brief  敵ポーン隊長のビヘイビアツリーのソースファイル
 *
 * @author CatCode
 *
 * @date    2026/01/23
 * 敵ポーン隊長のビヘイビアツリーオブジェクトクラス
 * 敵ポーン隊長の行動を決めるAIを定義する
 *
 * 2026/01/22
 * 作成
 * 
 * 2026/01/23
 * 攻撃を行うようにビヘイビアツリーを改修
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// 実装元
#include "EnemyPawnLeaderBT.h"

// 前方宣言
#include "EnemyPawnLeader.h"


// ビヘイビアツリー
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/CalculateDistanceToTargetTask.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/ChangeToChase.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/ChengeToWait.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/RequestChangeStateTask.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/CanAttack.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/ChangeToAttack.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Tasks/IfInAttackDistance.h>
#include <CCC/BehaviorTree/Composites/SelectorNode.h>
#include <CCC/BehaviorTree/Composites/SequenceNode.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Decorators/IfInDistance.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Decorators/IfOutDistance.h>
#include <CCC/BehaviorTree/EnemyPawnLeaderBT/Decorators/IfBeNotAttaking.h>


EnemyPawnLeaderBT::EnemyPawnLeaderBT(EnemyPawnLeader* object) :
	ObjectBase(),
	m_Blackboard()
{
	// オブジェクトから必要なポインタのみブラックボードへ写す
	m_Blackboard.p_Body            = object;
	m_Blackboard.p_MyTransform     = object->GetTransform();
	m_Blackboard.p_TargetTransform = object->GetTarget()->GetTransform();
	m_Blackboard.p_SkillStates     = object->GetSkillStatePointer();
}

EnemyPawnLeaderBT::~EnemyPawnLeaderBT() = default;

void EnemyPawnLeaderBT::Initialize()
{
	// タスクの作成
	auto calculate    = std::make_unique<CalculateDistanceToTargetTask>();
	auto chageChase   = std::make_unique<ChengeToChase>();
	auto chageWait    = std::make_unique<ChengeToWait>();
	auto request      = std::make_unique<RequestChangeStateTask>();
	
	// 攻撃処理のためのシーケンスを作成
	auto attackSequence = std::make_unique<CCC::BehaviorTree::Composites::SequenceNode>();
	attackSequence->AddChild(std::make_unique<IfInAttackDistance>());
	attackSequence->AddChild(std::make_unique<CanAttack>());
	attackSequence->AddChild(std::make_unique<ChengeToAttack>());

	// デコレータの作成
	auto ifin  = std::make_unique<IfInDistance>(std::move(chageChase));
	auto ifout = std::make_unique<IfOutDistance>(std::move(chageWait));

	// セレクターの作成
	auto selector = std::make_unique<CCC::BehaviorTree::Composites::SelectorNode>();
	selector->AddChild(std::move(attackSequence));
	selector->AddChild(std::move(ifin));
	selector->AddChild(std::move(ifout));

	// シーケンスの作成
	auto sequence = std::make_unique<CCC::BehaviorTree::Composites::SequenceNode>();
	sequence->AddChild(std::move(calculate));
	sequence->AddChild(std::move(selector));
	sequence->AddChild(std::move(request));

	// 攻撃中でない判定のデコレータを作成
	auto root = std::make_unique<IfBeNotAttaking>(std::move(sequence));

	// ルートに保存
	m_Root = std::move(root);
}

void EnemyPawnLeaderBT::Update(float)
{
	m_Root->Tick(m_Blackboard);
}

void EnemyPawnLeaderBT::Render()
{
}

void EnemyPawnLeaderBT::Finalize()
{
	m_Root.reset();
}

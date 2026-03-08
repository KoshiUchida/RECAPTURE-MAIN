/**
 * @file   BlockState.cpp
 *
 * @brief  キャラクターステートの「防御」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 * この状態で敵の攻撃を受けても多少のぶっ飛びは効かない
 *
 * 2026/01/28
 * 作成
 * 
 * 2026/02/27
 * パラディンキャラクターの実装に伴うアドレスの変更
 * 
 * 2026/03/08
 * 共通化に伴い基底クラスに沿って修正
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "BlockState.h"

// 管理クラス
#include <CCC/Managers/GameContextManager.h>

// コンポネート
#include <CCC/Objects/PawnCollider.h>

// このステートマシンを持つクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/AnimationType.h>
#include "../Paladin/PaladinParameter.h"

namespace CCC::States
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	BlockState::BlockState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner),
		m_OutSideCount(0)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		// アニメーションを「Paladin_Block」に変更するようリクエスト
		owner->GetAnimationState()->RequestAnimationChange(owner->GetAnimationTypeName(CCC::AnimationType::Block), 0.3f);
		owner->GetAnimationState()->ResetAnimationSpeed();

		// ベロシティを0にする
		owner->SetVelocity(DirectX::SimpleMath::Vector3::Zero);
	}

	BlockState::~BlockState() = default;

	void BlockState::Update(float)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		// 「死亡」行き
		if (owner->GetHP() <= 0.0f)
		{
			owner->RequestStateChange(CCC::StateType::Death);
			return;
		}

		// 「ノックバック」行き
		// 外力がある場合の処理
		DirectX::SimpleMath::Vector3 outSide = this->GetOwner()->GetComponent<CCC::Components::PawnCollider>("Collider")->GetOutsideForce();
		if (outSide != DirectX::SimpleMath::Vector3::Zero)
		{
			m_OutSideCount++;

			// 耐久能力を超える外力だったら「ノックバック」へ
			if (m_OutSideCount > CCC::PaladinParamerter::DEFENCE_COUNT ||
				outSide.Length() > CCC::PaladinParamerter::DEFENCE_POWER)
			{
				this->GetOwner()->SetVelocity(outSide);
				this->GetOwner()->RequestStateChange(CCC::StateType::Knockback);
				return;
			}

			// コンテキストに防御の成功した回数を足す
			CCC::Managers::GameContextManager* p_gcm = CCC::Managers::GameContextManager::GetInstance();
			p_gcm->SetGameData("BlockCount", p_gcm->GetDataAs<int>("BlockCount") + 1);
		}
	}
}

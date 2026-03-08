/**
 * @file   AttackState.cpp
 *
 * @brief  キャラクタークラスの「攻撃」状態のソースファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 *
 * 2026/01/05
 * アニメーション機能の問題で
 * アニメーションが終了しても終了が取れない問題の一時的な解決案として
 * １.3秒たったら強制的に切り替えるように機能を改修
 *
 * 2026/01/17
 * コメントを追加
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
#include "AttackState.h"

// このステートマシンを持つクラス
#include <CCC/Objects/Characters/CharacterBase.h>
#include <CCC/Objects/Characters/StateType.h>
#include <CCC/Objects/Characters/AnimationType.h>

namespace CCC::States
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	AttackState::AttackState(CCC::Bases::CharacterBase* p_Owner) :
		StateBase(p_Owner),
		m_Time(0.0f)
	{
		CCC::Bases::CharacterBase* owner = this->GetOwner();

		// アニメーションを「攻撃」に変更するようリクエスト
		owner->GetAnimationState()->RequestAnimationChange(owner->GetAnimationTypeName(CCC::AnimationType::Attack), 0.3f);
		owner->GetAnimationState()->ResetAnimationSpeed();
	}

	AttackState::~AttackState() = default;

	void AttackState::Update(float elapsedTime)
	{
		m_Time += elapsedTime;


		// アニメーションが終了するまたは、1.3秒の経過で「待機」状態に遷移するようリクエスト
		if (this->GetOwner()->GetAnimationState()->EndCurrentAnimation() || m_Time > 1.3f)
			this->GetOwner()->RequestStateChange(CCC::StateType::Idle);
	}
}

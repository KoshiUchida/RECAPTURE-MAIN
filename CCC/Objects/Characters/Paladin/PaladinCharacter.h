/**
 * @file   PaladinCharacter.h
 *
 * @brief  パラディンキャラクタークラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/03/08
 * パラディンキャラクターを定義するファイル
 * 正式名称：パラディンキャラクターオブジェクトクラス
 *
 * 2026/02/26
 * 作成
 * 
 * 2026/02/27
 * Pawnクラスの設計を参考に大幅の拡張
 * 
 * 2026/03/08
 * 共通化のため、大幅にソースコードを減らした
 */

// 多重インクルードガード
#pragma once

// 継承元クラス
#include <CCC/Objects/Characters/CharacterBase.h>

/// <summary>
/// パラディンキャラクターオブジェクトクラス
/// </summary>
class PaladinCharacter :
	public CCC::Bases::CharacterBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="teamID">チームID</param>
	/// <param name="p_PawnManager">ポーンの管理クラス</param>
	PaladinCharacter(const TeamID& teamID, PawnManager* p_PawnManager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PaladinCharacter();



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //
private:
	/// <summary>
	/// アニメーション関連の初期化
	/// </summary>
	void InitializeAnimationState() override;
};

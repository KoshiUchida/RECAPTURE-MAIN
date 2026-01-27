/**
 * @file   EnemyPawnLeaderStates.h
 *
 * @brief  敵Pawn隊長の状態数列のヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/23
 * 敵Pawn隊長のステートマシンで扱う状態の種別を定義する
 * 
 * 2026/01/22
 * 作成
 * 
 * 2026/01/23
 * 「攻撃」追加
 */

// 多重インクルードガード
#pragma once

/// <summary>
/// 敵隊長のステートマシンの状態
/// </summary>
enum class EnemyPawnLeaderStates
{
	Wait,	// 待機
	Chase,	// 追跡
	Attack,	// 攻撃
	Death,	// 死亡
};

/**
 * @file   EnemyPawnLeaderParameter.h
 *
 * @brief  敵ポーンリーダーオブジェクトクラスのパラメータヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/13
 *
 * 2026/01/13
 * 作成
 */

#pragma once

namespace EnemyPawnLeaderParameter
{
	static constexpr float MOVE_SPEED = 2.5f;	// 通常移動速度
	static constexpr float RUN_SPEED = 8.0f;	// ダッシュ速度
	static constexpr float STOP_RADIUS = 0.4f;	// 停止半径
	static constexpr float VELOCITY_CHANGE_SPEED = 0.7f;	// ベロシティの変化速度
	static constexpr float NEIGHBOR_DISTANCE = 3.0f;	// 近くと判断する距離

	// 陣形密度の値
	static constexpr float BREAK_LIMIT = 11.0f;	// 陣形を保てなくなる限界値
	static constexpr float STABLE_LIMIT = 3.0f;		// 安定する境界値
	static constexpr float DEATH_LIMIT = 20.0f;	// 陣形が修復不可能になる値

	// 陣形安定度の状態評価ライン
	namespace StabilityState
	{
		static constexpr float BROKEN = 0.15f;	// 15%未満なら崩壊している
		static constexpr float STABLE = 0.7f;	// 70%より上なら安定している

		static constexpr float RECOVERY_TIME = 2.5f;	// 陣形崩壊後の安定度を回復する時間
	}

	// 陣形の設定
	static constexpr int   NUMBER_PAWN = 15;	// 隊員数
	static constexpr float SPACING_BETWEEN_PAWN = 1.5f;	// 隊員間の間隔

	// スキルゲージ関連
	namespace SkillGauge
	{
		static constexpr float RECOVERY_RATE = 0.15f;	// １秒に対する回復率
		static constexpr float CONSUMPTION_RATE = 0.1f;		// スキルの発動中の１秒に対するゲージ消費量
		static constexpr float ROTATION_DEBUFF = 0.5f;		// スキル発動中の旋回速度のデバフ
		static constexpr float SPEED_BUFF = 1.5f;		// スキル発動中の速度バフ
	}
}

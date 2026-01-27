/**
 * @file   PawnParameter.h
 *
 * @brief  ポーンオブジェクトクラスのパラメータヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/25
 *
 * 2025/12/19
 * 作成
 * 
 * 2026/01/05
 * 走る速度を9.0から8.2に変更
 * 
 * 2026/01/06
 * とまる認識の半径を0.8から0.5に変更
 * 
 * 2026/01/25
 * 質量を追加
 */

#pragma once

namespace PawnParameter
{
	static constexpr float MAX_SPEED           = 3.0f;	// 歩行時最大速度
	static constexpr float MAX_RUN_SPEED       = 8.2f;	// 走行時最大速度
	static constexpr float RUN_RADIUS          = 3.5f;	// 走るべきだと判断する目標との距離
	static constexpr float SLOWING_RADIUS      = 2.0f;	// 遅くなる半径
	static constexpr float PREDICTION_TIME     = 0.5f;	// 目標座標を予測するためのどのぐらい先の時間を見るかの値（秒）
	static constexpr float STOP_RADIUS         = 0.5f;	// 目標に到達したと判別する半径
	static constexpr float MASS                = 95.0f;	// 質量
	
	// steering behiverのWanderの処理のための定数
	static constexpr float CIRCLE_DISTANCE     = 0.1f;	// 他のPawnとの距離
	static constexpr float CIRCLE_RADIUS       = 2.0f;	// 自身のパーソナルスペースの半径
	static constexpr float WANDER_JITTER       = 0.1f;
	
	static constexpr float MAX_SEE_AHEAD       = 5.0f;
	static constexpr float MAX_SEE_AHEAD_SMALL = 3.0f;
	static constexpr float MAX_AVOID_FORCE     = 5.0f;

	// スキル発動中の速度補正
	static constexpr float SPEED_BUFF = 1.5f;		// スキル発動中の速度バフ
}

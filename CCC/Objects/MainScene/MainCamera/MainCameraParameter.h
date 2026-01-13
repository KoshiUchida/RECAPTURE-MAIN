#pragma once

namespace MainCameraParameter
{
	// カメラの位置への距離の差分に対する割合
	static constexpr float EyeSpeedRate = 0.05f;

	// 注視点への距離の差分に対する割合
	static constexpr float TargetSpeedRate = 0.2f;

	// 水平方向の回転量
	static constexpr float HorizontalRotateRate = 180.0f;

	// 垂直方向の回転量
	static constexpr float VerticalRotateRate = 30.0f;

	// 垂直方向の限界値
	static constexpr float VerticalUpLimit   =  7.5f;
	static constexpr float VerticalDownLimit = -2.5f;



	// ---------------------------------------------------------------------- //
	// 初期化値
	// ---------------------------------------------------------------------- //
	namespace Initialize
	{
		// 座標
		static constexpr float PositionX =  0.0f;
		static constexpr float PositionY =  5.0f;
		static constexpr float PositionZ = 12.0f;

		// 目標に対するオフセットの座標
		static constexpr float TargetOffsetY = 5.0f;
	}
}

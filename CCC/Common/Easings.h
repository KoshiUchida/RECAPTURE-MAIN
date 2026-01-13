/**
 * @file   Easings.h
 *
 * @brief  イージング関数ヘッダーファイル
 *
 * @author CatCode
 *
 * @date
 * 2026/01/06
 * 
 * 2025/10/07
 * CCCのnamespaceを追加
 * 
 * 2026/01/06
 * easeInOutCubicを追加
 */

#pragma once
#include <cmath>
#include <numbers>

namespace CCC
{
	namespace Ease
	{
		static float OutQuart(const float& x) noexcept
		{
			return 1.0f - std::powf(1.0f - x, 4.0f);
		}

		static float InOutExpo(const float& x) noexcept
		{
			if (x <= 0.0f) return 0.0f;
			if (x >= 1.0f) return 1.0f;

			const float ax = (x < 0.5f) ? x : (1.0f - x);         // 対称化
			const float p = std::powf(2.0f, 20.0f * ax - 10.0f);  // powfは1回

			return (x < 0.5f) ? (0.5f * p) : (1.0f - 0.5f * p);
		}

		static float InOutElastic(const float& x) noexcept
		{
			if (x <= 0.0f) return 0.0f;
			if (x >= 1.0f) return 1.0f;

			constexpr float c5 = (2.0f * std::numbers::pi_v<float>) / 4.5f;

			const float s = (x < 0.5f) ? -0.5f : 0.5f;             // 前半は -1/2, 後半は +1/2
			const float ax = (x < 0.5f) ? x : (1.0f - x);          // 0.5からの“距離”に寄せる（対称化）
			const float p = std::powf(2.0f, 20.0f * ax - 10.0f);   // pow は1回
			const float w = std::sinf((20.0f * x - 11.125f) * c5); // sin も1回

			// 前半: -0.5 * p * w
			// 後半:  1 + 0.5 * p * w
			return (x < 0.5f) ? (s * p * w) : (1.0f + s * p * w);
		}

		static float InCirc(const float& x)
		{
			return 1.0f - std::sqrtf(1.0f - x * x);
		}

		static float InOutCubic(const float& x) noexcept
		{
			return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::powf(-2.0f * x + 2.0f, 3.0f) / 2.0f;
		}
	}
}

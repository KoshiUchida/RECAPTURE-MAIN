/**
 * @file   AnimationType.h
 *
 * @brief  アニメーションの種別クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/03/08
 *
 * 2026/03/08
 * 作成
 */

// 多重インクルードガード
#pragma once

namespace CCC
{
	/// <summary>
	/// アニメーションの種別
	/// </summary>
	enum class AnimationType
	{
		Idle,
		Walk,
		Run,
		Attack,
		FallingBack,
		StandUp,
		Death,
		Block,
	};
}

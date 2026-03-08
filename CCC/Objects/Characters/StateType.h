// パキャラクターステートの種別

// 多重インクルードガード
#pragma once

namespace CCC
{
	/// <summary>
	/// パラディンキャラクターステートの種別
	/// </summary>
	enum class StateType
	{
		None,
		Idle,
		Chase,
		Move,
		Attack,
		Knockback,
		StandUp,
		Block,
		Death,
	};
}

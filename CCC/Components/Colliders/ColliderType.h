/**
 * @file   ColliderType.h
 *
 * @brief  コライダーの種類を定義するのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2026/02/06
 *
 * コライダーの種類となるEnumクラス
 *
 * 2026/02/06
 * 作成
 */

// 多重インクルードガード
#pragma once

namespace CCC::Colliders
{
	enum class ColliderType
	{
		Circle,	// 円形
		Box,	// AABBの四角形
	};
}

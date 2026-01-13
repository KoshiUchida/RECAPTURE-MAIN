/**
 * @file   IState.h
 *
 * @brief  状態インターフェースのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2025/12/15
 * 
 * 状態遷移における状態は、必ずこのインターフェースを基底クラスとして実装しなければならない。
 */
 #pragma once

namespace CCC
{
	namespace Interfaces
	{
		class IState
		{
		public:
			virtual ~IState() = default;

			virtual void Update(float elpasedTime) = 0;
		};
	}
}

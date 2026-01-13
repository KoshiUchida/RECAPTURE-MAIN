#pragma once
#include <memory>
#include <random>

namespace CCC
{
	namespace Managers
	{
		class RandomManager final
		{
		private:
			static std::unique_ptr<RandomManager> s_Instance;

			RandomManager();

		public:
			static RandomManager* GetInstance();



		public:
			std::mt19937& GetMt19937();

			template<class T>
			T Rand(T min, T max)
			{
				// ®”Œ^‚Ìƒ‰ƒ“ƒ_ƒ€
				if constexpr (std::is_integral_v<T>)
				{
					std::uniform_int_distribution<T> dist(min, max);
					return dist(m_RandomNumberGenerator);
				}
				// •‚“®¬”“_Œ^‚Ìƒ‰ƒ“ƒ_ƒ€
				else if constexpr (std::is_floating_point_v<T>)
				{
					std::uniform_real_distribution<T> dist(min, max);
					return dist(m_RandomNumberGenerator);
				}
				// ŠÔˆá‚Á‚½Œ^‚ğ’e‚­
				else
				{
					static_assert(std::is_arithmetic_v<T>,
						"Rand<T>: T must be an arithmetic type");
				}
			}



		private:
			std::random_device m_RandomDevice;
			std::mt19937 m_RandomNumberGenerator;
		};
	}
}

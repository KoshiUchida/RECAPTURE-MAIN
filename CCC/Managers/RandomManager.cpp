#include "pch.h"
#include "RandomManager.h"

namespace CCC::Managers
{
	std::unique_ptr<RandomManager> RandomManager::s_Instance = nullptr;

	RandomManager::RandomManager() :
		m_RandomNumberGenerator(m_RandomDevice())
	{
	}

	RandomManager* RandomManager::GetInstance()
	{
		if (!s_Instance)
			s_Instance.reset(new RandomManager());

		return s_Instance.get();
	}



	std::mt19937& RandomManager::GetMt19937()
	{
		return m_RandomNumberGenerator;
	}
}

/**
 * @file   SceneBase.cpp
 *
 * @brief  シーンの基底クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/11/16
 */

#include "pch.h"
#include "SceneBase.h"

// シーンの管理クラス
#include <CCC/Managers/SceneManager.h>

namespace CCC::Bases
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	SceneBase::SceneBase() :
		mp_SceneManager{ CCC::Managers::SceneManager::GetInstance() }
	{
	}

	SceneBase::~SceneBase() noexcept = default;

	void SceneBase::ChangeScene(const std::string& nextSceneName)
	{
		if (mp_SceneManager)
			mp_SceneManager->RequestSceneChange(nextSceneName);
	}
}

/**
 * @file   ComponentManager.cpp
 *
 * @brief  コンポーネント管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/11
 *
 * コンポーネントを管理するためのクラス
 * 主な機能は更新または描画を行う必要があるコンポーネントクラスの更新処理または描画処理
 */

#include "pch.h"
#include "ComponentManager.h"

// 前方宣言	
#include <CCC/Interfaces/IUpdataComponent.h>

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<ComponentManager> ComponentManager::s_Instance = nullptr;

	ComponentManager::ComponentManager()
	{
	}

	ComponentManager* ComponentManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
			s_Instance.reset(new ComponentManager());

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
	ComponentManager::~ComponentManager()
	{
	}

	void ComponentManager::Update(float elapsedTime)
	{
		// 更新処理を持つコンポネート群を更新する
		for (CCC::Interfaces::IUpdataComponent* component : mp_UpdataComponets)
			component->Update(elapsedTime);
	}
}

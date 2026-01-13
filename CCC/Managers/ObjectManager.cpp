/**
 * @file   ObjectManager.cpp
 *
 * @brief  オブジェクトの管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2025/12/14
 * オブジェクトクラスの保持、生成、更新、描画、終了処理を管理するためのクラス
 * オブジェクトの生成指示は各シーンから行う
 */

#include "pch.h"
#include "ObjectManager.h"

namespace CCC::Managers
{
    // ---------------------------------------------------------------------- //
    // シングルトンパターン関連
    // ---------------------------------------------------------------------- //

    // 実体の初期化
    std::unique_ptr<ObjectManager> ObjectManager::s_Instance = nullptr;

    ObjectManager::ObjectManager()
    {
    }

    ObjectManager* ObjectManager::GetInstance()
    {
        // もし、実体がない場合に生成する
        if (!s_Instance)
            s_Instance.reset(new ObjectManager());

        // 実体へのポインタを返す
        return s_Instance.get();
    }



    // ---------------------------------------------------------------------- //
    // パブリック関数
    // ---------------------------------------------------------------------- //
    
    ObjectManager::~ObjectManager()
    {
        // オブジェクトの終了処理
        ObjectsFinalize();
    }

    void ObjectManager::Update(float elpsedTime)
    {
        // オブジェクト群の更新処理
        for (const std::pair<const std::string, std::unique_ptr<CCC::Interfaces::IObject>>& object : m_Objects)
        {
            object.second->Update(elpsedTime);
        }

        // 破棄リクエストが出ているオブジェクトを破棄
        for (const std::string& name : m_DeleteNames)
        {
            m_Objects[name]->Finalize();
            m_Objects.erase(name);
        }

        m_DeleteNames.clear();
    }
    
    void ObjectManager::Render()
    {
        for (const std::pair<const std::string, std::unique_ptr<CCC::Interfaces::IObject>>& object : m_Objects)
        {
            object.second->Render();
        }
    }
    void ObjectManager::ObjectsFinalize()
    {
        for (const std::pair<const std::string, std::unique_ptr<CCC::Interfaces::IObject>>& object : m_Objects)
        {
            object.second->Finalize();
        }

        m_Objects.clear();
    }
}

/**
 * @file   MainScene.h
 *
 * @brief  メインシーンのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/05
 * 
 * 2026/01/05
 * デバッグ表示を追加
 * PawnLeaderの前方宣言
 * PawnLeaderオブジェクトへのポインタを帆s時するメンバ変数の追加
 */
#pragma once

// 基底クラス
#include <CCC/Scenes/SceneBase.h>

// デバッグ機能
#include <CCC/Debug/DebugCamera.h>
#include <CCC/Debug/DebugFont.h>

// 前方宣言
namespace CCC::Managers
{
    class ResourceManager;
    class InputManager;
    class SceneManager;
}
class PawnLeader;

/// <summary>
/// サンプルシーン
/// </summary>
class MainScene :
    public CCC::Bases::SceneBase
{
public:
    MainScene();
    ~MainScene();

    void Initialize(                 ) override;
    void Update    (float elapsedTime) override;
    void Render    (                 ) override;
    void Finalize  (                 ) override;

private:
    // リソース管理クラスへのポインタ
    CCC::Managers::ResourceManager* mp_ResourceManager;

    // 入力管理クラスへのポインタ
    CCC::Managers::InputManager* mp_InputManager;

    // シーン管理クラスへのポインタ
    CCC::Managers::SceneManager* mp_SceneManager;



    // デバッグカメラへのポインタ
    std::unique_ptr<CCC::Debug::DebugCamera> m_DebugCamera;

    //	デバッグフォントへのポインタ
    std::unique_ptr<CCC::Debug::DebugFont> m_DebugFont;

    //	入力レイアウト 
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

    //	エフェクト 
    std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;

    //	プリミティブバッチ 
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
    
    // カメラモード
    enum class CameraMode
    {
        Debug,
        Main,
    } m_CameraMode;


    PawnLeader* mp_PawnLeader;
};


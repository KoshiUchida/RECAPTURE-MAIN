/**
 * @file   MainScene.h
 *
 * @brief  メインシーンのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/23
 * 
 * 2025/12/19
 * 作成
 * 
 * 2026/01/05
 * デバッグ表示を追加
 * PawnLeaderの前方宣言
 * PawnLeaderオブジェクトへのポインタを保持するメンバ変数の追加
 * 
 * 2026/01/18
 * コメントを追加
 * 
 * 2026/01/22
 * ビヘイビアツリーの状態変化を見るためにm_BTを追加
 * 
 * 2026/01/23
 * 勝敗判定のために敵オブジェクトを保持する変数を追加
 */

// 多重インクルードガード
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
class EnemyPawnLeader;

/// <summary>
/// サンプルシーン
/// </summary>
class MainScene :
    public CCC::Bases::SceneBase
{
    // ---------------------------------------------------------------------- //
    // パブリック関数
    // ---------------------------------------------------------------------- //
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    MainScene();

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~MainScene();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="elapsedTime">経過時間</param>
    void Update(float elapsedTime) override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Render() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;



    // ---------------------------------------------------------------------- //
    // メンバ関数
    // ---------------------------------------------------------------------- //
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

    // ポーン隊長オブジェクトクラスへのポインタ
    PawnLeader* mp_PawnLeader;

    // ビヘイビアツリーの状態のデバッグ表示
    std::wstring m_BT;

    // 敵
    EnemyPawnLeader* mp_EnemyFirst;
    EnemyPawnLeader* mp_EnemySecond;
};


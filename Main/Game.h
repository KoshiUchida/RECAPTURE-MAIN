//
// Game.h
// 
// 2026/01/21
// DirectX提供ライブラリをDXフォルダに移動
// それに伴い、インクルードのディレクトリ名を変更
//
// 2026/01/27
// オーディオ管理クラスの実装に伴い
// その管理クラスの前方宣言とクラスへのポインタを保持する変数を追加

#pragma once

#include <DX/DeviceResources.h>
#include <DX/StepTimer.h>

#include <CCC/Debug/DebugFont.h>

// 管理クラスの前方宣言
namespace CCC::Managers
{
    class ManagersHub;
    class SceneManager;
    class ResourceManager;
    class ComponentManager;
    class ObjectManager;
    class InputManager;
    class AudioManager;
}
#include <CCC/Managers/ColliderManager.h>


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

    // 画面モードを設定する関数（TRUE：フルスクリーン）
    void SetFullscreenState(BOOL value);

    // デバイスリソースを取得する関数
    DX::DeviceResources* GetDeviceResources();

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // 全画面モードにするか
    BOOL m_fullscreen;

    CCC::Managers::ManagersHub*      mp_ManagersHub;    // 管理クラスのハブ
    CCC::Managers::SceneManager*     mp_SceneManager;       // シーン管理クラスの実体へのポインタ
    CCC::Managers::ResourceManager*  mp_ResourceManager;    // リソース管理クラスの実体へのポインタ
    CCC::Managers::ComponentManager* mp_ComponentManager;   // コンポネート管理クラスの実体へのポインタ
    CCC::Managers::ObjectManager*    mp_ObjectManager;      // オブジェクト管理クラスの実体へのポインタ
    CCC::Managers::InputManager*     mp_InputManager;       // 入力管理クラスの実体へのポインタ
    CCC::Managers::AudioManager*     mp_AudioManager;       // オーディオ管理クラスの実体へのポインタ
    std::unique_ptr<CCC::Managers::ColliderManager> m_ColliderManager;  // コライダー管理クラス
};

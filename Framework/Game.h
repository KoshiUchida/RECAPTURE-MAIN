//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

#include <CCC/Debug/DebugFont.h>

// 管理クラスの前方宣言
namespace CCC::Managers
{
    class SceneManager;
    class ResourceManager;
    class ComponentManager;
    class ObjectManager;
    class InputManager;
}


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

    // シーン管理クラスの実体へのポインタ
    CCC::Managers::SceneManager* mp_SceneManager;

    // リソース管理クラスの実体へのポインタ
    CCC::Managers::ResourceManager* mp_ResourceManager;

    // コンポネート管理クラスの実体へのポインタ
    CCC::Managers::ComponentManager* mp_ComponentManager;

    // オブジェクト管理クラスの実体へのポインタ
    CCC::Managers::ObjectManager* mp_ObjectManager;

    // 入力管理クラスの実体へのポインタ
    CCC::Managers::InputManager* mp_InputManager;
};

//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

// ディスプレイ情報
#include <Main/DisplayInfo.h>

// 管理クラスのインクルード
#include <CCC/Managers/ManagersHub.h>
#include <CCC/Interfaces/IColliderManager.h>
#include <CCC/Managers/SceneManager.h>
#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/ComponentManager.h>
#include <CCC/Managers/ObjectManager.h>
#include <CCC/Managers/InputManager.h>
#include <CCC/Managers/AudioManager.h>

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false) :
    m_fullscreen       ( false ),
    mp_ManagersHub     ( nullptr ),
    mp_SceneManager    ( CCC::Managers::SceneManager    ::GetInstance() ),
    mp_ResourceManager ( CCC::Managers::ResourceManager ::GetInstance() ),
    mp_ComponentManager( CCC::Managers::ComponentManager::GetInstance() ),
    mp_ObjectManager   ( CCC::Managers::ObjectManager   ::GetInstance() ),
    mp_InputManager    ( CCC::Managers::InputManager    ::GetInstance() ),
    mp_AudioManager    ( CCC::Managers::AudioManager    ::GetInstance() )
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: デフォルトの可変タイムステップ モード以外のモードが必要な場合は、タイマー設定を変更します。
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / DisplayInfo::FranesPerSecond);

    // リソース管理クラスのデータをセット
    mp_ResourceManager->SetResources(m_deviceResources.get(), &m_timer);

    // コライダー管理クラスの生成
    m_ColliderManager = std::make_unique<CCC::Managers::ColliderManager>();

    // 管理クラスのハブを生成
    CCC::Managers::ManagersHub::Generate(
        m_ColliderManager.get()
    );
    mp_ManagersHub = CCC::Managers::ManagersHub::Get();
}

#pragma region Frame Update
// 基本的なゲームループの実行
void Game::Tick()
{
    if (m_deviceResources->ConsumeResizeRequest())
    {
        m_deviceResources->CreateWindowSizeDependentResources();
        CreateWindowSizeDependentResources(); // Game側の依存リソースがあるなら
    }

    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// 更新処理
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // 入力管理クラスの更新処理
    mp_InputManager->Update();

    // シーン管理クラスの更新処理
    mp_SceneManager->Update(elapsedTime);

    // オブジェクト管理クラスの更新処理
    mp_ObjectManager->Update(elapsedTime);

    // コンポネート管理クラスの更新処理
    mp_ComponentManager->Update(elapsedTime);

    // コライダー管理クラスの更新処理
    m_ColliderManager->Update();

    // オーディオ管理クラスの更新処理
    mp_AudioManager->Update();
}
#pragma endregion

#pragma region Frame Render
// 描画処理
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();



    // ---------------------------------------------------------------------- //
    // 描画処理の開始
    // ---------------------------------------------------------------------- //
    m_deviceResources->PIXBeginEvent(L"Render");

    // シーン管理クラスの描画処理
    mp_ObjectManager->Render();
    mp_SceneManager->Render();

    // ---------------------------------------------------------------------- //
    // 描画処理の終了
    // ---------------------------------------------------------------------- //
    m_deviceResources->PIXEndEvent();

    // 新しい画面を描画する
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::AliceBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
    if (m_fullscreen)
    {
        BOOL fullscreen = FALSE;
        if (SUCCEEDED(m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr)))
        {
            // DXGIによってフォーカスロストで一時的にウィンドウ化されていた場合、復元する
            if (!fullscreen)
            {
                m_deviceResources->GetSwapChain()->SetFullscreenState(TRUE, nullptr);
            }
        }
    }
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);

    // フルスクリーンか調べる
    BOOL fullscreen = FALSE;

    m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);

    // フルスクリーンじゃなくなったら（意図しないフォーカスロストなど）
    if (m_fullscreen && !fullscreen)
    {
        // 意図した全画面状態を維持するため、m_fullscreen は上書きしない
        m_deviceResources->CreateWindowSizeDependentResources();
    }
    else if (m_fullscreen != fullscreen)
    {
        m_fullscreen = fullscreen;

        // ResizeBuffers関数呼び出し
        m_deviceResources->CreateWindowSizeDependentResources();
    }
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width  = DisplayInfo::Width;
    height = DisplayInfo::Height;
}
void Game::SetFullscreenState(BOOL value)
{
    m_fullscreen = value;
    m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
    if (value) m_deviceResources->CreateWindowSizeDependentResources();
}
DX::DeviceResources* Game::GetDeviceResources()
{
    return m_deviceResources.get();
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    // TODO:もし、生成時にデバイスが必要ならここで生成する
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

/**
 * @file   SceneManager.cpp
 *
 * @brief  シーンの管理クラスのソースファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * 
 * シーンの登録はこのファイルで行う
 * 
 * 2025/12/12
 * シーンクラスのフォルダ構成変更と同時にインクルードのパスを修正
 * 
 * 2026/01/16
 * 一部コメントの修正と追加
 * 
 * 2026/01/18
 * タイトルシーンの追加
 * 終了フラグが立っているのに描画をすることが判明
 * 描画関数に終了フラグが立っている時は早期リターンするように改良
 * 
 * リザルトシーンの追加
 */

// プリコンパイル済みヘッダー
#include "pch.h"

// クラス定義元
#include "SceneManager.h"

// C++標準ライブラリ
#include <unordered_map>
#include <thread>
#include <functional>

// シーンインターフェース
#include <CCC/Interfaces/IScene.h>

// 他の管理クラス
#include <CCC/Managers/ResourceManager.h>
#include <CCC/Managers/ObjectManager.h>

// ---------------------------------------------------------------------- //
// 登録するシーン
// 
// 作成したシーンをこのソースファイルで読み込む
// シーンの登録はDefineScene関数で記述する
// ---------------------------------------------------------------------- //
#include <CCC/Scenes/MainScene.h>
#include <CCC/Scenes/SampleSceneSec.h>
#include <CCC/Scenes/TitleScene.h>
#include <CCC/Scenes/ResultScene.h>

namespace CCC::Managers
{
	// ---------------------------------------------------------------------- //
	// インプリメンテーションの宣言
	// ---------------------------------------------------------------------- //
	struct SceneManager::Impl
	{
		// ---------------------------------------------------------------------- //
		// メンバ関数
		// ---------------------------------------------------------------------- //

		/// <summary>
		/// データのリセット
		/// </summary>
		void Reset();

		/// <summary>
		/// シーンの登録
		/// </summary>
		void DefineScene();

		/// <summary>
		/// 起動時のシーンの設定
		/// </summary>
		/// <param name="startSceneName">シーン名</param>
		void SetStartScene(const std::string& startSceneName);

		/// <summary>
		/// シーンの変更処理
		/// </summary>
		void ChangeScene();



		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="elapsedTime">経過時間</param>
		void Update(float elapsedTime);

		/// <summary>
		/// 描画処理
		/// </summary>
		void Render();

		/// <summary>
		/// 現在のシーンの名前の取得
		/// </summary>
		/// <returns>現在のシーン名</returns>
		const std::string& GetCurrentSceneName() const {
			return m_CurrentSceneName;
		}

		/// <summary>
		/// 現在のシーンの名前の取得
		/// </summary>
		/// <returns>現在のシーン名（wstring型）</returns>
		std::wstring GetCurrentSceneNameUTF16() const {
			// m_CurrentSceneName は UTF-8 の std::string を想定
			// Windowsなら WinAPI で UTF-8 → UTF-16 変換が堅い
			const std::string& s = m_CurrentSceneName;

			if (s.empty()) return {};

			int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
			if (len <= 0) return {};

			std::wstring w;
			w.resize(len - 1); // 終端NULぶんを除いたサイズ
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, w.data(), len);
			return w;
		}


		/// <summary>
		/// シーン遷移の要求
		/// </summary>
		/// <param name="changeSceneName">遷移先のシーン名</param>
		void RequestSceneChange(const std::string& changeSceneName);

		/// <summary>
		/// 終了命令
		/// </summary>
		void End() { m_IsEnd = true; }

		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //

		// シーンの生成関数を保持するマップ
		using SceneFactoryMap = std::unordered_map<std::string, std::function<std::unique_ptr<CCC::Interfaces::IScene>()>>;
		SceneFactoryMap m_Factories;

		// 現在のシーン
		std::unique_ptr<CCC::Interfaces::IScene> m_CurrentScene;

		// 現在のシーンの名前
		std::string m_CurrentSceneName = "";

		// 変更要求のあったシーン
		std::string m_RequestedSceneName = "";

		// 終了命令
		bool m_IsEnd = false;

		// 再入/スレッド安全ガード
		bool m_InUpdate = false;
		std::thread::id m_MainThread;
	};

	// ---------------------------------------------------------------------- //
	// インプリメンテーションの宣言終了
	// ---------------------------------------------------------------------- //



	// ---------------------------------------------------------------------- //
	// シーンの定義をファクトリーに登録する
	// 
	// 使用するシーンはここで登録する
	// ---------------------------------------------------------------------- //
	void SceneManager::Impl::DefineScene()
	{
		using namespace std;

		// サンプルシーンの登録
		m_Factories["MainScene"] = []() {
			return make_unique<MainScene>();
		};

		// サンプルシーンセカンドの登録
		m_Factories["SampleSceneSec"] = []() {
			return make_unique<SampleSceneSec>();
		};

		// タイトルシーンの登録
		m_Factories["TitleScene"] = []() {
			return make_unique<TitleScene>();
		};

		// リザルトシーンの登録
		m_Factories["ResultScene"] = []() {
			return make_unique<ResultScene>();
		};



		// ---------------------------------------------------------------------- //
		// 開始シーンの設定
		// ---------------------------------------------------------------------- //
#if defined(_DEBUG)
			// デバッグ時の開始シーン
		SetStartScene("TitleScene");
#else
			// リリース時の開始シーン
		SetStartScene("TitleScene");
#endif
	}



	// ---------------------------------------------------------------------- //
	// シングルトンパターン関連
	// ---------------------------------------------------------------------- //

	// 実体の初期化
	std::unique_ptr<SceneManager> SceneManager::s_Instance = nullptr;

	SceneManager::SceneManager() :
		m_Impl(std::make_unique<Impl>())
	{
	}

	SceneManager* SceneManager::GetInstance()
	{
		// もし、実体がない場合に生成する
		if (!s_Instance)
		{
			s_Instance.reset(new SceneManager());
			s_Instance->m_Impl->Reset();
			s_Instance->m_Impl->DefineScene();

			// 最初にこのスレッドでしか動かさない契約
			s_Instance->m_Impl->m_MainThread = std::this_thread::get_id();
		}

		// 実体へのポインタを返す
		return s_Instance.get();
	}



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //

	void SceneManager::Impl::Reset()
	{
		m_RequestedSceneName = "";
		m_IsEnd = false;
	}

	void SceneManager::Impl::SetStartScene(const std::string& startSceneName)
	{
		RequestSceneChange(startSceneName);
	}

	void SceneManager::Impl::ChangeScene()
	{
		// リソース管理クラスの取得
		CCC::Managers::ResourceManager* p_rm = CCC::Managers::ResourceManager::GetInstance();

		// オブジェクト管理クラスの取得
		CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();

		// 現在のシーンの終了処理
		if (m_CurrentScene)
		{
			m_CurrentScene->Finalize();

			// オブジェクト群の終了処理
			p_om->ObjectsFinalize();

			// 対応するリソースの破棄
			p_rm->DeleteResources(m_CurrentSceneName);
		}

		// 新しいシーンを生成する
		m_CurrentScene = m_Factories[m_RequestedSceneName]();

		// 対応するリソースの読み込み
		p_rm->LoadResources(m_RequestedSceneName);

		// 新しいシーンの初期化処理
		m_CurrentScene->Initialize();

		// 現在のシーン名を新しいシーン名に変更
		m_CurrentSceneName = m_RequestedSceneName;

		// 要求されているシーン名をリセット
		m_RequestedSceneName = "";
	}



	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //

	SceneManager::~SceneManager() = default;

	void SceneManager::Update(float elapsedTime) {
		m_Impl->Update(elapsedTime);
	}

	void SceneManager::Render() {
		m_Impl->Render();
	}

	const std::string& SceneManager::GetCurrentSceneName() const {
		return m_Impl->GetCurrentSceneName();
	}

	std::wstring SceneManager::GetCurrentSceneNameUTF16() const {
		return m_Impl->GetCurrentSceneNameUTF16();
	}

	void SceneManager::RequestSceneChange(const std::string& changeSceneName) {
		m_Impl->RequestSceneChange(changeSceneName);
	}

	void SceneManager::End() {
		m_Impl->End();
	}



	// ---------------------------------------------------------------------- //
	// Implのパブリック関数
	// ---------------------------------------------------------------------- //

	void SceneManager::Impl::Update(float elapsedTime)
	{
		assert(std::this_thread::get_id() == m_MainThread && "SceneManager::Update must run on the main thread.");

		// すでにUpdate中なら再入を拒否
		if (m_InUpdate) return;

		// この関数が終了すると必ず再入を許可する
		m_InUpdate = true;
		struct ResetFlag {
			bool& f;
			~ResetFlag() { f = false; }
		} reset{ m_InUpdate };

		// シーンの変更要求があった場合はシーンの変更を行う
		if (m_RequestedSceneName != "")
		{
			ChangeScene();
		}

		// 現在のシーンを更新
		if (m_CurrentScene)
		{
			// TODO:後にObjectManagerを実装したら修正
			//s_ObjectManager->Update(elapsedTime);
			m_CurrentScene->Update(elapsedTime);

			//#if defined(_DEBUG)
							// TODO:入力管理のクラスができたら修正ないしは削除する
							// ESCキーで強制終了 このゴミ機能が
			if (DirectX::Keyboard::Get().GetState().Escape)
				End();
			//#endif
		}

		// もし、終了命令があった場合
		if (!m_IsEnd) return;

		// 現在のシーンの終了
		if (m_CurrentScene)
			m_CurrentScene->Finalize();

		// オブジェクト管理クラスの取得
		CCC::Managers::ObjectManager* p_om = CCC::Managers::ObjectManager::GetInstance();
		p_om->ObjectsFinalize();

		// ゲームを終了させる
		// TODO:イベント管理クラスにこの機能は変更する
		PostQuitMessage(0);
	}

	void SceneManager::Impl::Render()
	{
		if (m_IsEnd) return;

		// 現在のシーンの実体が存在する場合
		if (m_CurrentScene)
		{
			// 現在のシーンを描画
			m_CurrentScene->Render();
		}
	}

	void SceneManager::Impl::RequestSceneChange(const std::string& changeSceneName)
	{
		// 要求するシーンが登録されているか
		if (m_Factories.count(changeSceneName) == 0)
		{
			// エラーメッセージ
			MessageBox(
				NULL,
				L"登録されていないシーンを要求されています。",
				L"SceneManager", MB_OK);
			return;
		}

		m_RequestedSceneName = changeSceneName;
	}
}

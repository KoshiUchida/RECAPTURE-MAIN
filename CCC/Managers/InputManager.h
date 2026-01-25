/**
 * @file   InputManager.h
 *
 * @brief  入力の管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/21
 * 入力機器から必要に応じて入力情報を取得する管理クラス
 * 必ず、更新処理を行わなければならない
 * 
 * 2025/12/14
 * 作成
 * 
 * 2026/01/21
 * コメント内容に間違いがあったため、コメントを修正
 * namespaceを簡略化
 */

// 多重インクルードガード
#pragma once

// C++標準ライブラリ
#include <any>

namespace CCC::Managers
{
	/// <summary>
	/// 入力管理クラス
	/// </summary>
	class InputManager
	{
		// ---------------------------------------------------------------------- //
		// シングルトンパターン関連
		// ---------------------------------------------------------------------- //
	private:
		// 実体を他のクラスが作れないように

		/// <summary>
		/// コンストラクタ
		/// </summary>
		InputManager();

		// このクラスの実体
		static std::unique_ptr<InputManager> s_Instance;


	public:
		/// <summary>
		/// 実体の取得
		/// </summary>
		/// <returns>リソース管理クラスへのポインタ</returns>
		static InputManager* GetInstance();



		// ---------------------------------------------------------------------- //
		// パブリック関数
		// ---------------------------------------------------------------------- //
	public:
		~InputManager();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// マウスの状態を取得
		/// </summary>
		DirectX::Mouse::State GetMouseState() const {
			return m_MouseState;
		}



		// ---------------------------------------------------------------------- //
		// メンバ関数
		// ---------------------------------------------------------------------- //
	private:
		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 入力情報を登録する処理
		/// </summary>
		void Registration();



		// ---------------------------------------------------------------------- //
		// 入力管理関連
		// ---------------------------------------------------------------------- //
	private:
		// 入力値取得関数の集まり
		using InputCollection = std::unordered_map < std::string, std::function<std::any()>>;
		InputCollection m_Inputs;

		/// <summary>
		/// 入力情報の登録
		/// </summary>
		/// <param name="name">登録名</param>
		/// <param name="fanction">登録する入力</param>
		void AddInputs(const std::string& name, const std::function<std::any()>& fanction);

		/// <summary>
		/// 入力情報の取得
		/// </summary>
		/// <param name="name">登録名</param>
		std::any GetInput(const std::string& name);

	public:
		/// <summary>
		/// 入力情報の取得
		/// </summary>
		/// <typeparam name="T">登録した関数の返し値の型</typeparam>
		/// <param name="name">登録名</param>
		template<class T>
		T GetInputAs(const std::string& name)
		{
			try {
				return std::any_cast<T>(GetInput(name));
			}
			catch (...) {
				return T{};
			}
			
		}



		// ---------------------------------------------------------------------- //
		// メンバ変数
		// ---------------------------------------------------------------------- //
	private:
		// マウスへのポインタ
		DirectX::Mouse* mp_Mouse;

		// マウスの状態
		DirectX::Mouse::State m_MouseState;

		// キーボードへのポインタ
		DirectX::Keyboard* mp_Keyboard;

		// キーボードの状態
		DirectX::Keyboard::State m_KeyboardState;
	};
}


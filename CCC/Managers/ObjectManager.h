/**
 * @file   ObjectManager.h
 *
 * @brief  オブジェクトの管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/14
 * オブジェクトクラスの保持、生成、更新、描画、終了処理を管理するためのクラス
 * オブジェクトの生成指示は各シーンから行う
 * 
 * 機能が多いので、特に描画機能を分離するべきだ
 */

#pragma once

// 管理対象
#include <CCC/Interfaces/IObject.h>

namespace CCC
{
	namespace Managers
	{
		class ObjectManager
		{
			// ---------------------------------------------------------------------- //
			// シングルトンパターン関連
			// ---------------------------------------------------------------------- //
		private:
			// 実体を他のクラスが作れないように

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ObjectManager();

			// このクラスの実体
			static std::unique_ptr<ObjectManager> s_Instance;


		public:
			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns>リソース管理クラスへのポインタ</returns>
			static ObjectManager* GetInstance();



			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			~ObjectManager();

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elpsedTime">経過処理</param>
			void Update(float elpsedTime);

			/// <summary>
			/// 描画処理
			/// </summary>
			void Render();

			/// <summary>
			/// オブジェクト群の一斉終了処理
			/// </summary>
			void ObjectsFinalize();

			/// <summary>
			/// オブジェクトの生成指示
			/// </summary>
			/// <typeparam name="T">オブジェクトクラス</typeparam>
			/// <typeparam name="...Args">オブジェクトクラスのコンストラクタ</typeparam>
			/// <param name="name">登録名</param>
			/// <param name="...args">オブジェクトクラスのコンストラクタ</param>
			template<class T, class... Args>
			T* CreateObject(const std::string& name, Args&&... args)
			{
				auto object = std::make_unique<T>(std::forward<Args>(args)...);
				object->Initialize();
				m_Objects.emplace(name, std::move(object));

				return dynamic_cast<T*>(m_Objects[name].get());
			}

			/// <summary>
			/// 特定オブジェクトの破棄リクエスト
			/// </summary>
			void RequestDeleteObject(const std::string& name)
			{
				m_DeleteNames.push_back(name);
			}
			
			/// <summary>
			/// オブジェクトの取得
			/// </summary>
			/// <typeparam name="T">取得するオブジェクトのクラス</typeparam>
			/// <param name="name">登録名</param>
			template<class T>
			T* GetObject(const std::string& name)
			{
				return dynamic_cast<T*>(m_Objects[name].get());
			}



			// ---------------------------------------------------------------------- //
			// メンバ変数
			// ---------------------------------------------------------------------- //
		private:
			// オブジェクトのマップ
			using ObjectCollection = std::unordered_map<std::string, std::unique_ptr<CCC::Interfaces::IObject>>;
			ObjectCollection m_Objects;

			// 破棄リクエストのオブジェクト名群
			std::vector<std::string> m_DeleteNames;
		};
	}
}

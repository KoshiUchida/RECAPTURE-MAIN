/**
 * @file   ObjectBase.h
 *
 * @brief  オブジェクト基底クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/14
 */

#pragma once
#include <CCC/Interfaces/IObject.h>
#include <CCC/Interfaces/IComponent.h>

namespace CCC
{
	namespace Bases
	{
		/// <summary>
		/// オブジェクト基底クラス
		/// </summary>
		class ObjectBase :
			public CCC::Interfaces::IObject
		{
			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			ObjectBase();

			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~ObjectBase();

			/// <summary>
			/// コンポネートの追加
			/// </summary>
			/// <typeparam name="T">コンポネートクラス</typeparam>
			/// <typeparam name="...Args">コンポネートクラスのインスタンス</typeparam>
			/// <param name="name">登録名</param>
			/// <param name="...args">コンポネートクラスのインスタンス</param>
			template<class T, class... Args>
			T* AddComponent(const std::string& name, Args&&... args)
			{
				auto component = std::make_unique<T>(std::forward<Args>(args)...);
				m_Components.emplace(name, std::move(component));
				return dynamic_cast<T*>(m_Components[name].get());
			}

			/// <summary>
			/// コンポネート取得
			/// </summary>
			/// <typeparam name="T">コンポネートへのポインタ</typeparam>
			/// <param name="name">登録名</param>
			/// <returns>コンポネートへのポインタ</returns>
			template<class T>
			T* GetComponent(const std::string& name)
			{
				auto it = m_Components.find(name);
				if (it == m_Components.end())
					return nullptr;

				return dynamic_cast<T*>(it->second.get());
			}

		private:
			// コンポネートのマップ
			std::unordered_map<std::string, std::unique_ptr<CCC::Interfaces::IComponent>> m_Components;
		};
	}
}


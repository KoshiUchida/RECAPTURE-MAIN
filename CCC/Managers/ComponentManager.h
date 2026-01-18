/**
 * @file   ComponentManager.h
 *
 * @brief  コンポーネント管理クラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/12
 *
 * コンポーネントを管理するためのクラス
 * 主な機能は更新または描画を行う必要があるコンポーネントクラスの更新処理または描画処理
 * 
 * TODO:描画関連の機能を実装
 */

#ifndef COMPONENT_MANAGER_DEFINED
#define COMPONENT_MANAGER_DEFINED

namespace CCC
{
	// 管理するインターフェースの前方宣言
	namespace Interfaces
	{
		class IUpdataComponent;
	}

	namespace Managers
	{
		/// <summary>
		/// コンポーネント管理クラス
		/// </summary>
		class ComponentManager final
		{
			// ---------------------------------------------------------------------- //
			// シングルトンパターン関連
			// ---------------------------------------------------------------------- //
		private:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			ComponentManager();

		public:
			/// <summary>
			/// 実体の取得
			/// </summary>
			/// <returns>コンポネート管理クラスへのポインタ</returns>
			static ComponentManager* GetInstance();

		private:
			// このクラスの実体
			static std::unique_ptr<ComponentManager> s_Instance;



			// ---------------------------------------------------------------------- //
			// パブリック関数
			// ---------------------------------------------------------------------- //
		public:
			/// <summary>
			/// デストラクタ
			/// </summary>
			virtual ~ComponentManager();

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="elapsedTime">経過時間</param>
			void Update(float elapsedTime);



			// ---------------------------------------------------------------------- //
			// 更新処理を持つコンポネート関連
			// ---------------------------------------------------------------------- //
		private:
			// 更新処理を持つコンポネートへのポインタ群
			std::vector<CCC::Interfaces::IUpdataComponent*> mp_UpdataComponets;

		public:
			/// <summary>
			/// 更新処理を持つコンポネートを登録する
			/// </summary>
			void AddUpdataComponent(CCC::Interfaces::IUpdataComponent* p_Component)
			{
				mp_UpdataComponets.push_back(p_Component);
			}

			/// <summary>
			/// 更新処理を持つコンポネートを解除する
			/// </summary>
			void SubUpdataComponent(CCC::Interfaces::IUpdataComponent* p_Component);



			// ---------------------------------------------------------------------- //
			// 描画処理を持つコンポネート関連
			// ---------------------------------------------------------------------- //

			// TODO:描画コンポーネントを実装したら作成する
		};
	}
}

#endif // !COMPONENT_MANAGER_DEFINED

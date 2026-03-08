/**
 * @file   BoxCollider.h
 *
 * @brief  ボックスコライダーコンポネートクラスのヘッダーファイル
 *
 * @author CatCode
 *
 * @date   2026/02/08
 *
 * AABBの形式で処理するボックスコライダーコンポネートクラス
 * 
 * 2026/02/06
 * 作成
 * 
 * 2026/02/08
 * 取得関数の実装
 */

 // 多重インクルードガード
#pragma once

// 基底クラス
#include "ColliderComponentBase.h"

namespace CCC::Components
{
    /// <summary>
    /// ボックスコライダーコンポネートクラス
    /// </summary>
    class BoxCollider :
        public CCC::Bases::ColliderComponentBase
    {
        // ---------------------------------------------------------------------- //
        // パブリック関数
        // ---------------------------------------------------------------------- //
    public:
        /// <summary>
        /// コンストラクタ
        /// </summary>
		/// <param name="p_Owner">オーナーへのポインタ</param>
		/// <param name="p_Transform">トランスフォームコンポネートへのポインタ</param>
		/// <param name="isStatic">静的なコライダーか？</param>
		/// <param name="width">横の大きさ</param>
		/// <param name="isStatic">縦の大きさ</param>
        BoxCollider(
            CCC::Interfaces::IObject*   p_Owner,
            CCC::Components::Transform* p_Transform,
            bool                        isStatic,
            float                       width,
            float                       height
        );

        /// <summary>
        /// デストラクタ
        /// </summary>
        virtual ~BoxCollider() noexcept;



        // ---------------------------------------------------------------------- //
        // 取得関数
        // ---------------------------------------------------------------------- //
        
        /// <summary>
        /// 横の大きさの取得
        /// </summary>
        float GetWidth() const;

        /// <summary>
        /// 縦の大きさの取得
        /// </summary>
        float GetHeight() const;

        /// <summary>
        /// 大きさの取得
        /// </summary>
        DirectX::SimpleMath::Vector2 GetSize() const;



        // ---------------------------------------------------------------------- //
        // メンバ変数
        // ---------------------------------------------------------------------- //
    private:
        float m_Width;    // 横の大きさ
        float m_Height;   // 縦の大きさ
    };
}

/**
 * @file   Map.h
 *
 * @brief  マップオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/02/04
 *
 * 2026/02/04
 * 作成
 */

// 多重インクルードガード
#pragma once

// 基底クラス
#include <CCC/Objects/ObjectBase.h>

/// <summary>
/// マップオブジェクトクラス
/// </summary>
class Map final:
    public CCC::Bases::ObjectBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Map();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Map();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override final;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override final;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render() override final;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override final;
};

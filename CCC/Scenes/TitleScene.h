/**
 * @file   TitleScene.h
 *
 * @brief  タイトルシーンのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/18
 *
 * 2026/01/18
 * 作成
 */

// 多重インクルードガード
#pragma once

// 親クラス
#include <CCC/Scenes/SceneBase.h>

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene final :
	public CCC::Bases::SceneBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TitleScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;
};


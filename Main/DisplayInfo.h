/**
 * @file   DisplayInfo.h
 *
 * @brief  ディスプレイの情報を定義するヘッダファイル
 *
 * @author CatCode
 *
 * @date   2025/12/14
 */

#pragma once

/// <summary>
/// ディスプレイ情報
/// </summary>
namespace DisplayInfo
{
	/// <summary>
	/// ディスプレイの横幅
	/// </summary>
	static constexpr int Width  = 1920;
	
	/// <summary>
	/// ディスプレイの縦幅
	/// </summary>
	static constexpr int Height = 1080;

	/// <summary>
	/// スクリーンの横幅
	/// </summary>
	static int ScreenWidth = 1920;

	/// <summary>
	/// スクリーンの縦幅
	/// </summary>
	static int ScreenHeight = 1080;

	/// <summary>
	/// 画面比率
	/// </summary>
	static constexpr float AspectRatio = Width / Height;

	/// <summary>
	/// フレームレート
	/// </summary>
	static constexpr int FranesPerSecond = 60;
}

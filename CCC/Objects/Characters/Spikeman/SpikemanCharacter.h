
// 多重インクルードガード
#pragma once

// 継承元クラス
#include <CCC/Objects/Characters/CharacterBase.h>

class SpikemanCharacter :
	public CCC::Bases::CharacterBase
{
	// ---------------------------------------------------------------------- //
	// パブリック関数
	// ---------------------------------------------------------------------- //
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="teamID">チームID</param>
	/// <param name="p_PawnManager">ポーンの管理クラス</param>
	SpikemanCharacter(const TeamID& teamID, PawnManager* p_PawnManager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SpikemanCharacter();



	// ---------------------------------------------------------------------- //
	// メンバ関数
	// ---------------------------------------------------------------------- //
private:
	/// <summary>
	/// アニメーション関連の初期化
	/// </summary>
	void InitializeAnimationState() override;
};


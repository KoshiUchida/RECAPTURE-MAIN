/**
 * @file   SKillGaugeUI.h
 *
 * @brief  スキルゲージUIオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date   2026/01/08
 *
 * 2026/01/08
 * 作成
 */

// 多重インクルードガード
#pragma once

// 基底クラス
#include <CCC/Objects/ObjectBase.h>

// 前方宣言
namespace CCC
{
	namespace Resources
	{
		class TextureResource;
	}

	namespace Managers
	{
		class ResourceManager;
	}
}
class PawnLeader;

/// <summary>
/// スキルゲージUIオブジェクトクラス
/// </summary>
class SkillGaugeUI final:
	public CCC::Bases::ObjectBase
{
public:
	SkillGaugeUI();
	~SkillGaugeUI();

	void Initialize() override final;
	void Update(float elapsedTime) override final;
	void Render()override;
	void Finalize() override;

	void SetTarget(PawnLeader* p_Target) {
		mp_Target = p_Target;
	}

private:
	// 管理クラス
	CCC::Managers::ResourceManager* mp_ResourceManager;	// リソース管理クラス

	// テクスチャリソース
	CCC::Resources::TextureResource* mp_Frame;			// 枠
	CCC::Resources::TextureResource* mp_Bar;			// バー
	CCC::Resources::TextureResource* mp_Label_Gladius;	// ラベル：グラディウス
	CCC::Resources::TextureResource* mp_Bottom_Q;		// ボタン：Q

	// UIの表示する値を持つオブジェクト
	PawnLeader* mp_Target;
};

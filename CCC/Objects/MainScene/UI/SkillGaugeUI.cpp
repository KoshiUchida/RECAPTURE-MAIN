#include "pch.h"
#include "SkillGaugeUI.h"

#include <Framework/DisplayInfo.h>
#include <CCC/Common/Easings.h>

// リソースクラス
#include <CCC/ResourceClasses/TextureResource.h>

// 管理クラス
#include <CCC/Managers/ResourceManager.h>

// 関係のあるオブジェクト
#include <CCC/Objects/MainScene/PawnLeader/PawnLeader.h>

SkillGaugeUI::SkillGaugeUI() :
	mp_ResourceManager(CCC::Managers::ResourceManager::GetInstance()),
	mp_Frame        (nullptr),
	mp_Bar          (nullptr),
	mp_Label_Gladius(nullptr),
	mp_Bottom_Q     (nullptr),
	mp_Target       (nullptr)
{
}

SkillGaugeUI::~SkillGaugeUI() = default;

void SkillGaugeUI::Initialize()
{
	// リソースの取得
	mp_Frame         = mp_ResourceManager->GetResource<CCC::Resources::TextureResource>("SkillGauge_Frame");
	mp_Bar           = mp_ResourceManager->GetResource<CCC::Resources::TextureResource>("SkillGauge_Bar");
	mp_Label_Gladius = mp_ResourceManager->GetResource<CCC::Resources::TextureResource>("SkillGauge_Label_Gladius");
	mp_Bottom_Q      = mp_ResourceManager->GetResource<CCC::Resources::TextureResource>("SkillGauge_Bottom_Q");
}

void SkillGaugeUI::Update(float elapsedTime)
{
	elapsedTime;
}

void SkillGaugeUI::Render()
{
	// スキルゲージに必要な値の取得
	PawnLeader::SkillStates skillState = mp_Target->GetSkillState();
	float                   skillGauge = mp_Target->GetSkillGauge();

	// スキルゲージの状態でバーの色を変化
	DirectX::XMVECTORF32 barColor;
	switch (skillState)
	{
	case PawnLeader::SkillStates::Inactive:
		barColor = DirectX::Colors::Blue;
		break;
	case PawnLeader::SkillStates::Active:
		barColor = DirectX::Colors::Yellow;
		break;
	case PawnLeader::SkillStates::Recovering:
	case PawnLeader::SkillStates::Count:
	default:
		barColor = DirectX::Colors::Cyan;
		break;
	}


	// ---------------------------------------------------------------------- //
	// 描画処理
	// ---------------------------------------------------------------------- //

	// スプライトバッチの取得
	DirectX::DX11::SpriteBatch* spriteBatch = mp_ResourceManager->GetSpriteBatch();

	// バーの描画
	spriteBatch->Begin();

	spriteBatch->Draw(
		mp_Bar->Get(),
		DirectX::SimpleMath::Vector2(DisplayInfo::Width - 417.0f, DisplayInfo::Height - 62.0f),
		nullptr,
		DirectX::Colors::Black,
		0.0f,
		DirectX::SimpleMath::Vector2::Zero,
		DirectX::SimpleMath::Vector2(0.5f, 0.5f)
	);

	spriteBatch->Draw(
		mp_Bar->Get(),
		DirectX::SimpleMath::Vector2(DisplayInfo::Width - 417.0f, DisplayInfo::Height - 62.0f),
		nullptr,
		barColor,
		0.0f,
		DirectX::SimpleMath::Vector2::Zero,
		DirectX::SimpleMath::Vector2(skillGauge * 0.5f, 0.5f)
	);
	spriteBatch->End();


	// バー以外の描画
	spriteBatch->Begin();

	spriteBatch->Draw(
		mp_Frame->Get(),
		DirectX::SimpleMath::Vector2(DisplayInfo::Width - 425.0f, DisplayInfo::Height - 67.0f),
		nullptr,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2::Zero,
		DirectX::SimpleMath::Vector2(0.5f, 0.5f)
	);

	spriteBatch->Draw(
		mp_Label_Gladius->Get(),
		DirectX::SimpleMath::Vector2(DisplayInfo::Width - 380.0f, DisplayInfo::Height - 77.0f),
		nullptr,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2::Zero,
		DirectX::SimpleMath::Vector2(0.5f, 0.5f)
	);

	spriteBatch->Draw(
		mp_Bottom_Q->Get(),
		DirectX::SimpleMath::Vector2(DisplayInfo::Width - 75.0f, DisplayInfo::Height - 75.0f),
		nullptr,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2::Zero,
		DirectX::SimpleMath::Vector2(0.5f, 0.5f)
	);

	spriteBatch->End();
}

void SkillGaugeUI::Finalize()
{
}

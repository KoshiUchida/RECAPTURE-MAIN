#include "pch.h"
#include "SampleSceneSec.h"

SampleSceneSec::SampleSceneSec() :
	CCC::Bases::SceneBase()
{
}

SampleSceneSec::~SampleSceneSec() = default;

void SampleSceneSec::Initialize()
{
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);
}

void SampleSceneSec::Update(float elapsedTime)
{
	elapsedTime;

	if (DirectX::Keyboard::Get().GetState().O)
		ChangeScene("MainScene");
}

void SampleSceneSec::Render()
{
}

void SampleSceneSec::Finalize()
{
}

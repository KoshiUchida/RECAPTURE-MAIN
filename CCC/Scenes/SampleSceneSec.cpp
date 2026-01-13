#include "pch.h"
#include "SampleSceneSec.h"

SampleSceneSec::SampleSceneSec() :
	CCC::Bases::SceneBase()
{
}

SampleSceneSec::~SampleSceneSec() = default;

void SampleSceneSec::Initialize()
{
}

void SampleSceneSec::Update(float elapsedTime)
{
	elapsedTime;

	if (DirectX::Keyboard::Get().GetState().O)
		ChangeScene("SampleScene");
}

void SampleSceneSec::Render()
{
}

void SampleSceneSec::Finalize()
{
}

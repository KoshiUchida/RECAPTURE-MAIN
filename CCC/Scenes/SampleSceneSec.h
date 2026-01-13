#pragma once
#include <CCC/Scenes/SceneBase.h>

class SampleSceneSec final:
    public CCC::Bases::SceneBase
{
public:
    SampleSceneSec();
    ~SampleSceneSec();

    void Initialize() override;
    void Update(float elapsedTime) override;
    void Render() override;
    void Finalize() override;
};


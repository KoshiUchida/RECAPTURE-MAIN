// 多重インクルードガード
#pragma once

// 親クラス
#include <CCC/Scenes/SceneBase.h>

class SampleSceneSec final:
    public CCC::Bases::SceneBase
{
public:
    SampleSceneSec();
    virtual ~SampleSceneSec();

    void Initialize() override;
    void Update(float elapsedTime) override;
    void Render() override;
    void Finalize() override;
};


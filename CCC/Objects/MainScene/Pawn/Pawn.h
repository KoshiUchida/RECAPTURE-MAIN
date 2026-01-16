/**
 * @file   Pawn.h
 *
 * @brief  ポーンオブジェクトクラスのヘッダファイル
 *
 * @author CatCode
 *
 * @date    2026/01/05
 *
 * 2025/12/17
 * 実装
 * 
 * 2025/12/19
 * ステアリングビヘイビアーのメンバ関数を追加
 * 
 * 2025/12/21
 * ステアリングビヘイビアーのメンバ関数を改修
 * 
 * 2026/01/05
 * 陣形の所定の位置との差分を保持させる
 * 上記の取得関数を実装
 */

#pragma once
#include <CCC/Objects/PawnBase.h>
#include "States/PawnState.h"

#include <CCC/Objects/TeamID.h>

// 前方宣言
namespace CCC
{
    namespace Components
    {
        class Transform;
    }
}
class Paladin;
class PawnManager;

class Pawn :
    public CCC::Bases::PawnBase
{
public:
	Pawn(const TeamID& teamID, PawnManager* p_PawnManager);
	~Pawn();

	void Initialize() override final;
    void Process(float elapsedTime) override final;

    void SetTarget(PawnBase* p_OtherPawn) {
        mp_Target = p_OtherPawn;
    }

    void SetOffset(const DirectX::SimpleMath::Vector3& offset) {
        m_Offset = offset;
	}

    float GetMaxSpeed();

    PawnBase* GetTarget() const {
        return mp_Target;
    }

    DirectX::SimpleMath::Vector3 GetOffset() const {
        return m_Offset;
	}

    float GetDiffToTarget() const {
        return m_DiffToTarget;
    }

    void AddNeighbor(PawnBase* p_OtherPawn) {
        m_Neighbors.push_back(p_OtherPawn);
	}

    void RequestStateChange(const std::string& state)
    {
		m_RequestState = state;
	}

    /// <summary>
    /// 走っているかのフラグを設定
    /// </summary>
    void SetRunning(bool is)
    {
        m_IsRunning = is;
    }

    /// <summary>
    /// 陣形安定度の設定
    /// </summary>
    void SetFormationStability(float stability)
    {
        m_FormationStability = stability;
    }

    /// <summary>
    /// 陣形安定度の取得
    /// </summary>
    float GetFormationStability() const {
        return m_FormationStability;
    }

public:
    DirectX::SimpleMath::Vector3 Seek   (const DirectX::SimpleMath::Vector3& targetPosition);
    DirectX::SimpleMath::Vector3 Flee   (const DirectX::SimpleMath::Vector3& targetPosition);
    DirectX::SimpleMath::Vector3 Arrive (const DirectX::SimpleMath::Vector3& targetPosition);
    DirectX::SimpleMath::Vector3 Pursuit(const DirectX::SimpleMath::Vector3& targetPosition, const DirectX::SimpleMath::Vector3& targetVelocity);
    DirectX::SimpleMath::Vector3 Wander ();

    void ResetNeighbors() {
        m_Neighbors.clear();
    }

private:
    // 速度
    float m_Speed;

    // ターゲット
    PawnBase* mp_Target;

    // ランダム角度
    float m_Theta;

	// 隊列の位置
	DirectX::SimpleMath::Vector3 m_Offset;

    // 今走るか?
	bool m_IsRunning;

	// 近くにいる他のポーンベースの全て
	std::vector<PawnBase*> m_Neighbors;

    // シーンの生成関数を保持するマップ
    using PawnStateFactoryMap = std::unordered_map<std::string, std::function<std::unique_ptr<PawnState>()>>;
    PawnStateFactoryMap m_Factories;

    std::unique_ptr<PawnState> m_State;

    // リクエストステート
	std::string m_RequestState;

    // 目標との差分
    float m_DiffToTarget;

    // 現在の陣形安定度
    float m_FormationStability;
};

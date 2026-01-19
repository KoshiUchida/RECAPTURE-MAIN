/**
 * @file   DefaultPS.hlsl
 *
 * @brief  デフォルトピクセルシェーダのHLSLファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * 標準ピクセルシェーダ
 *
 * 2026/01/18
 * 作成
 */

// 共通定義
#include "Common.hlsli"

Texture2D    tex       : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // そのままの色を返す
    return tex.Sample(samLinear, input.texcoord);
}

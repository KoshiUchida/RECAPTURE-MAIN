/**
 * @file   DefaultVS.hlsl
 *
 * @brief  デフォルト頂点シェーダのHLSLファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * 標準頂点シェーダ
 *
 * 2026/01/18
 * コメント追加
 */

// 共通定義
#include "Common.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    // スクリーン上の座標を計算
    float2 screen = screenAndTextureSize.xy;
    float2 ndc;
    ndc.x = (input.Pos.x / screen.x) * 2.0f - 1.0f;
    ndc.y = 1.0f - (input.Pos.y / screen.y) * 2.0f;

    output.position = float4(ndc, 0.0f, 1.0f);

    output.color    = input.Color;
    output.texcoord = input.Tex;
    return output;
}
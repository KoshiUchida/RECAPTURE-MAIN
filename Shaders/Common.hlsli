/**
 * @file   Common.hlsli
 *
 * @brief  共通定義のHLSLIファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * シェーダの共通定義
 *
 * 2026/01/18
 * コメント追加
 */

// 定数バッファの定義
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
    float4 screenAndTextureSize;
    float4 diffuse;
};

// screenAndTextureSize
// x : screenWidth
// y : screenHeight
// z : textureWidth
// w : textureHeight


// 頂点シェーダの入力定義
struct VS_INPUT
{
    float3 Pos   : POSITION;
    float4 Color : COLOR;
    float2 Tex   : TEXCOORD;
};

// ピクセルシェーダの入力定義
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

/**
 * @file   DisplayGS.hlsl
 *
 * @brief  画面上のジオメトリシェーダのHLSLファイル
 *
 * @author CatCode
 *
 * @date   2026/01/18
 * 画面上のジオメトリシェーダ
 *
 * 2026/01/18
 * コメント追加
 */

// 共通定義
#include "Common.hlsli"

// 頂点数
static const int vnum = 4;

static const float4 offset_array[vnum] =
{
    float4(-1.0f,  1.0f, 0.0f, 0.0f), //	左上
	float4( 1.0f,  1.0f, 0.0f, 0.0f), //	右上
	float4(-1.0f, -1.0f, 0.0f, 0.0f), //	左下
	float4( 1.0f, -1.0f, 0.0f, 0.0f), //	右下

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream<PS_INPUT> output
)
{
    float2 sizePx = screenAndTextureSize.zw; // すでにスケール済み texW, texH
    float2 halfPx = sizePx * 0.5f;

    // ピクセル量 → NDC量へ
    float2 halfNDC;
    halfNDC.x = (halfPx.x / screenAndTextureSize.x) * 2.0f;
    halfNDC.y = (halfPx.y / screenAndTextureSize.y) * 2.0f;

    // center は「すでに clip 空間」前提なら input[0].position を中心とする
    float4 center = input[0].position;

    for (int i = 0; i < 4; i++)
    {
        PS_INPUT element;

        float2 ndcOffset = offset_array[i].xy * halfNDC;
        element.position = center + float4(ndcOffset, 0, 0);

        // UV
        element.texcoord = float2(
            (offset_array[i].x + 1.0f) * 0.5f,
            (-offset_array[i].y + 1.0f) * 0.5f
        );

        element.color = input[0].color;
        output.Append(element);
    }
    output.RestartStrip();

}
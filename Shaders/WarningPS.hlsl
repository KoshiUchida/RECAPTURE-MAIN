#include "Common.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.texcoord;

    float t = diffuse.y; // time
    float on = saturate(diffuse.z); // warningフラグ(0/1)

    // フラグOFFなら通常描画
    float4 base = tex.Sample(samLinear, uv) * input.color;
    if (on <= 0.0f)
        return base;

    // 点滅（0..1）
    float blink = sin(t * 6.0f) * 0.5f + 0.5f;

    // ライン単位の横ブレ（軽い信号乱れ）
    float lineFloor = floor(uv.y * 120.0f);
    float jitter = (random(float2(lineFloor, t)) - 0.5f) * 0.01f * blink;
    uv.x += jitter;

    float4 col = tex.Sample(samLinear, uv) * input.color;

    // 明滅をアルファに反映（派手すぎるなら係数下げる）
    col.a *= lerp(0.35f, 1.0f, blink);

    return col;
}

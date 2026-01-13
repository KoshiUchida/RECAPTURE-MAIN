#include "Common.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float easeInOutExpo(float x = 0.0f)
{
    if (x <= 0.0f)
        return 0.0f;
    if (x >= 1.0f)
        return 1.0f;

    const float ax = (x < 0.5f) ? x : (1.0f - x); // 対称化
    const float p = pow(2.0f, 20.0f * ax - 10.0f); // powfは1回

    return (x < 0.5f) ? (0.5f * p) : (1.0f - 0.5f * p);
}

float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}


float4 oldTV(float2 inUV)
{
    float2 uv = inUV;
    //中心をずらす
    uv = uv - 0.5f; // uv float2(0.5f,0.5f)
    float vignette = length(uv);
    
    // タイムの取得
    float time = diffuse.y;
    
    float stability = saturate(1.0f - diffuse.x); // 0..1（悪化ほど大きい想定）
    float state = saturate(1.0f); // 0/1（状態でON/OFF：WARNING/BROKEN時だけ 1 推奨）
    float k = stability * state; // 実効強度

    

    float2 texUV = uv;
    float jitterAmp = lerp(0.002f, 0.025f, k); // 安定→小、崩壊→大
    texUV.x += (random(float2(floor(uv.y * 120.0f), time)) - 0.5f) * jitterAmp;

    float4 base = tex.Sample(samLinear, texUV);
    
    // 色ズレ（必要ならkで強められる）
    float ch = lerp(0.0000f, 0.004f, k);
    float3 col;
    col.r = tex.Sample(samLinear, texUV).r;
    col.g = tex.Sample(samLinear, texUV + float2(ch * 0.5f, 0)).g;
    col.b = tex.Sample(samLinear, texUV + float2(ch, 0)).b;
    
    float lineRate = lerp(0.0002f, 0.010f, k); // 崩壊ほど頻発
    float lineStep = step(random(float2(floor(texUV.y * 500.0f), time)), lineRate);

    
    float3 lineRGB = float3(
        random(uv + float2(123 + time, 0)),
        random(uv + float2(123 + time, 1)),
        random(uv + float2(123 + time, 2))
    );

    col = lerp(col, lineRGB, lineStep);
    
    base = float4(col, base.a);
    
    return base;
}

float4 main(PS_INPUT input) : SV_Target
{
    float2 uv = input.texcoord;
    float4 r = oldTV(uv);

    float stability = saturate(diffuse.x); // 0..1 充填率
    float time = diffuse.y;
    stability += sin(uv.y * 30.0f + time * 10.0f) * 0.01f * saturate(1.0f);

    float edge = 0.01f; // 境界の太さ（好みで）

    // 境界からの距離で 0..1 を作る（左=0, 右=1）
    float t = saturate((uv.x - (stability - edge)) / (2.0f * edge));

    // 境界だけ滑らかに（外は0 or 1に張り付く）
    float fill = easeInOutExpo(t);

    // uv.x < s なら塗り、uv.x > s なら黒、境界だけなめらか
    // fill は「黒へ寄る量」にしたいならそのまま使える
    float blackAmount = (uv.x > stability) ? 1.0f : 0.0f;
    blackAmount = lerp(0.0f, 1.0f, blackAmount); // 明示（不要だが読みやすく）

    // 境界帯では blackAmount を fill に置き換え
    if (abs(uv.x - stability) <= edge)
        blackAmount = fill;

    return lerp(r * input.color, float4(0, 0, 0, 1), blackAmount);
}

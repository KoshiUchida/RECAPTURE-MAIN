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


struct VS_INPUT
{
    float3 Pos   : POSITION;
    float4 Color : COLOR;
    float2 Tex   : TEXCOORD;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

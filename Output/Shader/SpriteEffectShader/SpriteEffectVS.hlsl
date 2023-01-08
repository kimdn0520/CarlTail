// SkyBox�� ȭ�鿡 �׸��� ���� ���̴� �ڵ��Դϴ�.

#include "SpriteEffectTypes.hlsli"

// �⺻���� ��� ����
cbuffer cbPerObject : register(b0)
{
    float4x4 WorldViewProj;
};

cbuffer cbUVOffset : register(b1)
{
    float2 UVOffset;
    float2 pad;
};

// ���ؽ� ���̴��� ������ ��
struct VS_Input
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output) 0;

    // �⺻���� ������ ������ ���ش�.
    vout.Pos = mul(float4(vin.PosL, 1.0f), WorldViewProj);

    // Offset�� ������ UV���� ��ȯ���ش�.
    vout.Tex.x = vin.Tex.x + UVOffset.x;
    vout.Tex.y = vin.Tex.y + UVOffset.y;

    return vout;
}
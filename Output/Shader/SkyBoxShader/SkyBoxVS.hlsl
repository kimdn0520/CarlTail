// SkyBox�� ȭ�鿡 �׸��� ���� ���̴� �ڵ��Դϴ�.
	

#include "SkyBoxTypes.hlsli"

// �⺻���� ��� ����
cbuffer cbPerFrame : register( b0 )
{
    float4x4 WorldViewProj;
};


// ���ؽ� ���̴��� ������ ��
struct VS_Input
{
    float3 PosL : POSITION;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;

    // z/w = 1�� �ǵ��� (�ϴ� ���� ��鿡 �ֵ���) z = w�� �����Ѵ�.
    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj).xyww;

    // ���� ���ؽ��� ��ġ�� �ؽ����� ��ȸ ���ͷ� ����Ѵ�.
    vout.PosL = vin.PosL;

    return vout;
}
// SkyBox�� �׸��� ���� �ȼ� ���̴� �ڵ� �Դϴ�.

#include "SkyBoxTypes.hlsli"

// ť���
TextureCube CubeMap : register( t0 );

// ���÷�
SamplerState samTriLinearSam : register( s0 );

// RasterizerState�� DepthStencilState�� ��� �ۿ��� �������ش�..

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    return CubeMap.Sample(samTriLinearSam, pin.PosL);
}
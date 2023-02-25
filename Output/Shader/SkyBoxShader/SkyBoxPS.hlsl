// SkyBox�� �׸��� ���� �ȼ� ���̴� �ڵ� �Դϴ�.

#include "SkyBoxTypes.hlsli"

// ť���
TextureCube CubeMap : register( t0 );

// ���÷�
SamplerState samTriLinearSam : register( s0 );

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    float4 color = CubeMap.Sample(samTriLinearSam, pin.PosL);

    // Gamma Correction
    color = pow(color, 2.2f);

    return color;
}
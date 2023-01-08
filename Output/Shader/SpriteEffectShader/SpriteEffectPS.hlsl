// SkyBox�� �׸��� ���� �ȼ� ���̴� �ڵ� �Դϴ�.

#include "SpriteEffectTypes.hlsli"

// ť���
Texture2D EffectMap : register( t0 );

// ���÷�
SamplerState Sampler : register( s0 );

// RasterizerState�� DepthStencilState�� ��� �ۿ��� �������ش�..

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    // �ؽ��ĸ� �����ؼ� �ٷ� �������� �ȴ�.
    return EffectMap.Sample(Sampler, pin.Tex);
}
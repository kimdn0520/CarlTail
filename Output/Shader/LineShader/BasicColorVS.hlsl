// �⺻���� ��ġ�� ������ ������ �ִ� PosColor�� vertex Shader �Դϴ�.
	// �⺻������ Line ���� �׸� ���� ��� �����Դϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
// �� ��� ���۸� �޾ƾ���.
#include "BasicColorTypes.hlsli"

// ����� View�� ������ �ǹ̰�..
	// ����� ������Ʈ��ü�� ��ǥ
	// View�� proj�� ī�޶� ���� ����� �� ����..
// �ϴ� ������ �̷��� �صΰ�, ���߿� �ʿ信 ���� �����ϴ� ������ �Ѵ�.

// �⺻���� ��� ����
cbuffer cbPerFrame : register(b0)
{
    float4x4 WorldViewProj;
};

// ���ؽ� ���̴��� ������ ��
struct VS_Input
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;
    vout.Pos = mul(float4(vin.PosL, 1.0f), WorldViewProj);
    vout.Color = vin.Color;

    return vout;
}
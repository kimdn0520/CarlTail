// �⺻���� ��ġ�� ������ ������ �ִ� PosColor�� pixel Shader �Դϴ�.
	// LineShader�� �̸��� �����߽��ϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
// �� ��� ���۸� �޾ƾ���.
#include "LineTypes.hlsli"

// �÷��� �ȼ����� �ٲ��ݴϴ�. constant Buffer�� ���� �޾Ƽ� �װ� Return
cbuffer LineColor : register( b1 )
{
    float4 Color;
}

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    return Color;
}
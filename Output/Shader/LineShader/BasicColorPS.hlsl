// �⺻���� ��ġ�� ������ ������ �ִ� PosColor�� pixel Shader �Դϴ�.
	// LineShader�� �̸��� �����߽��ϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
// �� ��� ���۸� �޾ƾ���.
#include "BasicColorTypes.hlsli"

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    return pin.Color;
}
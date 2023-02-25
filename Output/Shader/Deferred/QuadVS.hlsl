// Deferred ���� ������ ȭ�鿡 �׸��� �׷��ֱ� ���� �簢��  Vertex Shader �Դϴ�.

// ���ؽ� ���̴��� ������ ��
struct VS_Input
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct VS_Output
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout;
    vout.PosH = float4(vin.PosL, 1.0f).xyww;
	vout.Tex = vin.Tex;

    return vout;
}
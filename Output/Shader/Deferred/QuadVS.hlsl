// Deferred ���� ������ ȭ�鿡 �׸��� �׷��ֱ� ���� �簢��  Vertex Shader �Դϴ�.

// ���ؽ� ���̴��� ������ ��
struct VS_Input
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};

struct VS_Output
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout;
    vout.PosH = float4(vin.PosL, 1.0f).xyww;
	vout.Tex = vin.Tex;

    return vout;
}
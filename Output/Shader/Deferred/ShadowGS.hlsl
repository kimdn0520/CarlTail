// PointLight Shadow Map�� ���� Geometry Shader �Դϴ�.
	// �⺻������ ���� ���ؽ��� �׸��� ������ ������ش�. => ���� �������� ������.
	// ����Ʈ ����Ʈ������ ���� �������� �׸��� ť��ʿ� ���� �׷��ش�
	// => 3���� ������ �� ��(6��)�� ���� => 18���� ��������

// ���ؽ� ���̴��� ������ ��
	// PBR Object�� ���ؼ��� �׸��ڸ� ����� �� ���̴�.
	// �⺻������ line, Effect, SkyBox�� ������ ��κ��� ��ü�� PBR shader�� ���� ������ �ȴ�.

// ť����� �� �鿡 �ش��ϴ� shadowMatrix
cbuffer cbLightMatrix : register(b1)
{
    float4x4 LightViewProj[6];
}

// ���� ���̴��� Input / Output => �ϴ��� TexCoord�� �����ϰ� ������.
struct GS_Input
{
	float4 PosH : SV_POSITION;
};

struct GS_Output
{
    float4 FragPos : FRAGPOS;
    float4 PosH : SV_POSITION;
    uint RTIndex : SV_RENDERTARGETARRAYINDEX;
};

// ���� ���̴� => ��� ���� ť������� �̷���� �׸��� ���� �ȴ�.
[maxvertexcount(18)]
void main(
	triangle GS_Input gin[3],
	inout TriangleStream<GS_Output> gout
)
{
	// ��� �鿡 ���ؼ� �׸��� ���̸��� �־��ش�..
    for (int face = 0; face < 6; ++face)
    {
        GS_Output element;
        element.RTIndex = face;

        // �������� �����ϴ� �� ������ �� ������ �����Ѵ�(PointLight �� ���� ViewProj�� ����Ѵ�.)
        for (uint i = 0; i < 3; i++)
        {
            element.FragPos = gin[i].PosH;
            element.PosH = mul(gin[i].PosH, LightViewProj[face]);
            gout.Append(element);
        }
        gout.RestartStrip();
    }

}
// PBRModel ���� �ʿ��� �⺻���� Ÿ���� ��Ÿ���ϴ�.
// ���ؽ� ���̴� ���� �ȼ� ���̴��� �˾ƾ��ϴ� ����ü�� �����մϴ�.
// == vout�� ����ϴ�.

struct VS_Output
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR0;

    float metallic : METALLIC;
    float roughness : ROUGHNESS;
};
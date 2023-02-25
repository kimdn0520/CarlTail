// Shadow Map�� ���� Vertex Shader �Դϴ�.
	// �⺻������ ���� ���ؽ��� �׸��� ������ ������ش�. => ���� �������� ������.

// ���ؽ� ���̴��� ������ ��
	// PBR Object�� ���ؼ��� �׸��ڸ� ����� �� ���̴�.
	// �⺻������ line, Effect, SkyBox�� ������ ��κ��� ��ü�� PBR shader�� ���� ������ �ȴ�.

// �⺻���� ��� ����
cbuffer cbPerObject : register(b0)
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}

// �׸��� �� ������ �ʿ��� Light�� View Proj   
cbuffer cbLightViewProj : register(b1)
{
    float4x4 LightViewProj;
}

// Bone Matrix
cbuffer cbBoneMatrix : register(b2)
{
    matrix BoneMatrix[100];
}

// TODO : Shadow�� ���� LightViewProj �߰�
#ifdef Skinned
struct VS_Input
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float3 TangentL : TANGENT;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;

	float4 Weight1 : WEIGHT;
	float4 Weight2 : WEIGHTS;

	uint4 BoneIndex1 : BONEINDEX;
	uint4 BoneIndex2 : BONEINDICES;
};

// ���ؽ� ���̴��� ������ ��
// ��ġ, �븻, Tangent ��
#else
struct VS_Input
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};
#endif

// ���� ���� ������ ���� ������ �ȴ�.
struct VS_Output
{
    float4 PosH : SV_POSITION;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout;

#ifdef Skinned
	// Skin Vertex������ Bone TM�� ���� �̵��� ���ؽ��� ��ġ�� �̾Ƴ� �ش�.
    float3 pos = float3(0.f, 0.f, 0.f);

    float totalWeight = (float)0.f;

    for (int k = 0; k < 4; k++)
    {
        totalWeight += vin.Weight1[k];

        totalWeight += vin.Weight2[k];
    }

    // ���� ȥ��
    for (int i = 0; i < 4; i++)
    {
        if(vin.BoneIndex1[i] != -1)
            pos += (vin.Weight1[i] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex1[i]]).xyz;
    }

    for (int j = 0; j < 4; j++)
    {
        if(vin.BoneIndex2[j] != -1)
			pos += (vin.Weight2[j] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex2[j]]).xyz;
    }

   
	vout.PosH = mul(float4(pos, 1.0f), World);
#else

  		// �켱 ���� ��ǥ��� �������� �̵� ��
    vout.PosH = mul(float4(vin.PosL, 1.0f), World);
#endif

    // ���� ������ ���� ��ũ�θ� �и����ش�.
		// DirLight �� ���� LightViewProj�� �����ְ�
    	// �ƴ� ���� ���� ������ �ʴ´� => ���� ���̴����� �� �鿡 ���ؼ� shadow Map�� ������ش�.
#ifdef DirLightShadow
		// Light�� viewProj�� ������ �̵���Ų��.
    vout.PosH = mul(vout.PosH, LightViewProj);
#endif

    return vout;
}
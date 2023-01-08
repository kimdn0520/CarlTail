// Ref.by https://github.com/CrustPizza/LevelUpEngine/blob/main/OutPut/Shader/PBRModel/PBRModelVS.hlsl
// Ref.by https://github.com/kimdn0520/GraphicsEngine_DX11/blob/main/Output/Data/Shader/Model_PBR_VS.hlsl

// PBR���� �� ������ �ϴ� PBR Model�� ����� ���� Vertex Shader�Դϴ�.
	// ���ο� Skinning�� ���õ� ���� �������� �� �ֽ��ϴ�.
#include "PBRModelTypes.hlsli"

// �⺻���� ��� ����
cbuffer cbPerObject : register( b0 )
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}

// PBR�� ���õ� �Ķ����.
cbuffer cbMaterial : register(b1)
{
    float Metallic;
    float Roughness;
    float2 pad1;

    float3 AddColor;
    float pad2;

    float3 EmissiveColor;
    float pad3;
	
    bool IsLight;
}

// Bone Matrix
cbuffer cbBoneMatrix : register( b2 )
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

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;

#ifdef Skinned
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);

    // Total Weight ���
    float totalWeight = (float)0.f;

    for (int k = 0; k < 4; k++)
	{
		totalWeight += vin.Weight1[k];

		totalWeight += vin.Weight2[k];
	}

	// Vertex Blending
	for (int i = 0; i < 4; i++)
	{
		// �븻 ��ȯ�� ��ȯ ��Ŀ� ��յ� ��ʰ� ���ٰ� �����Ѵ�.
        // ����ġ ����� �ƴ� ��ȯ����� �״�� ���.
        
        // �����ǿ��� ����Ʈ ����ŭ�� �����ش�. ���⼭ Ʈ������ ���� ������ (����� �ȹ޴´ٸ�?) 0�� ������ ��.
		if (vin.BoneIndex1[i] != -1)
		{
			pos += (vin.Weight1[i] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex1[i]]).xyz;
			normal += (vin.Weight1[i] / totalWeight) * mul(vin.NormalL, (float3x3)BoneMatrix[vin.BoneIndex1[i]]);
		}
	}

	for (int j = 0; j < 4; j++)
	{
		// �������� �������� ���ٰ� �����ϹǷ�, �븻���� ���� ������� ��ġ����� �ʿ����.
		// �� ��° ������ blending => �� ������ ������ ��ġ�� ���� 8���� ����
		if (vin.BoneIndex2[j] != -1)
		{
			pos += (vin.Weight2[j] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex2[j]]).xyz;
			normal += (vin.Weight2[j] / totalWeight) * mul(vin.NormalL, (float3x3)BoneMatrix[vin.BoneIndex2[j]]);
		}
	}

	vout.PosW = mul(float4(pos, 1.0f), World).xyz;

	vout.PosH = mul(float4(pos, 1.0f), WorldViewProj);

	// �������Ϳ� �������Ͱ� ������ �������Ǵ� ���� ���� ��ȯ����� ����ġ����� �븻(��������)�� �����ش�.
	vout.NormalW = normalize(mul(normal, (float3x3)WorldInvTranspose));
#else
    vout.PosW = mul(float4(vin.PosL, 1.0f), World).xyz;

    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj);

    vout.NormalW = normalize(mul(vin.NormalL, (float3x3) WorldInvTranspose));
#endif
    vout.Tex = vin.Tex;

    vout.TangentW = mul(vin.TangentL, (float3x3) World);

    vout.Color = vin.Color;

    vout.metallic = Metallic;
    vout.roughness = Roughness;

    return vout;
}
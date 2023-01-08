// PBR ���� �׷��ִ� shader �Դϴ�. ���۵带 ���� �� ������ ���� �ʾҽ��ϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
#include "PBRModelTypes.hlsli"
#include "PBRLightUtils.hlsli"

// �ؽ��� ��
Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D MetallicMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D AmbientOcclusionMap : register(t4);
Texture2D EmissiveMap : register(t5);

// ���÷�
SamplerState Sampler : register(s0);

// PBR�� ���õ� �Ķ����.
cbuffer cbMaterial : register(b0)
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

// outPut
	// ����� Render Ÿ�ٿ� ������ ������ Output ���ݴϴ�.
struct PS_Output
{
    // albedo => Texture��
    float4 Albedo : SV_TARGET0;
    // depth
    float4 DMRAO : SV_TARGET1; // Depth, Metallic, Roughness, Ambient Occlusion
    // normal
    float4 Normal : SV_TARGET2;
    // ����
    float4 WorldPos : SV_TARGET3;
    // emissive
    float4 Emissive : SV_TARGET4;
};

// �ȼ� ���̴�
PS_Output main(VS_Output pin) : SV_Target
{
    float4 color = pin.Color;
    float metallic = pin.metallic;
    float roughness = pin.roughness;
    float ambientOcclusion = 1.f;
    float4 emissive = float4(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef USE_ALBEDO
    color = pow(AlbedoMap.Sample(Sampler, pin.Tex), 2.2f);
#else
    color.rgb += AddColor.rgb;
#endif

#ifdef USE_NORMAL
    float3 normalMapSample = NormalMap.Sample(Sampler, pin.Tex).rgb;
    pin.NormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
#else
    pin.NormalW = normalize(pin.NormalW);
#endif

#ifdef USE_METALLIC
    metallic = MetallicMap.Sample(Sampler, pin.Tex);
#endif

#ifdef USE_ROUGHNESS
    roughness = RoughnessMap.Sample(Sampler, pin.Tex);
#endif

#ifdef USE_AO
    ambientOcclusion = AmbientOcclusionMap.Sample(Sampler, pin.Tex);
#endif

#ifdef USE_EMISSIVE
    emissive = EmissiveMap.Sample(Sampler, pin.Tex);
#endif

    PS_Output pout = (PS_Output)0;

    pout.DMRAO = float4(pin.PosH.z, metallic, roughness, ambientOcclusion);
    pout.Normal = float4(pin.NormalW, 1.0f);

    // �� ���� ����
    pout.WorldPos = float4(pin.PosW, IsLight);

    pout.Albedo = color;

    pout.Emissive = emissive;

    return pout;
}
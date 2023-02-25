// PBR �� ���꿡�� �ʿ��� ���� �Լ����� ��Ƶ׽��ϴ�.

// ���۷��� : https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl

// Specular coefficiant - fixed reflectance value for non-metals
#define F_ZERO float3(0.04f, 0.04f, 0.04f)
#define PI          3.14159265359f
#define EPSILON 0.000001f

#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		30
#define SPOT_LIGHT_COUNT		30

// Unity Spec ������ ������ ���� ���� ���..
#define kDielectricSpec float4(0.04f, 0.04f, 0.04f, 1.0f - 0.04f) // standard dielectric reflectivity coef at incident angle (= 4%)

// Light Informations For PBR
struct DirectionalLight
{
    float3 color; // ���� ��
    float power; // ���� ����

    float3 direction; // ����
    float pad;

    float4x4 lightViewProj;
};

struct PointLight
{
    float3 color;
    float power;

    float3 position; // ��ġ
    float range; // ����

    float3 pad;
    float isShadow;   // �׸��� ����

	// �� �鿡 ���� 6���� ��Ʈ������ ����
    float4x4 lightViewProj[6];
};

struct SpotLight
{
    float3 color;
    float power;

    float3 direction; // ����
    float innerAngle; // ���������� ���� ���踦 �־��� ��ġ

    float2 pad;
    float outerAngle; // Spot Angle => ����Ʈ ����Ʈ�� ������ ����
    float isShadow;   // �׸��� ���� 1.0f ���� 0.f ����

    float3 position; // ��ġ
    float range; // ������ ����

    float4x4 lightViewProj;
};

/// Point Light �׸��ڸ� ���� �Լ� ���� ����
float CalcDepthInShadow(const in float3 fragPos, float range);
float CalcPointShadowFactor(SamplerComparisonState samShadow, int index, TextureCubeArray cubeShadowMap, float4 shadowPosH, float4 textureInfo, float range);
float3 SRGBToLinear(float3 color);
float4 SRGBToLinear(float4 color);

// Diffuse BRDF
// Burley B. "Physically Based Shading at Disney"
// SIGGRAPH 2012 Course: Practical Physically Based Shading in Film and Game Production, 2012.
float3 Disney_Diffuse(in float roughnessPercent, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
    float energyBias = lerp(0.0f, 0.5f, roughnessPercent);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughnessPercent);

    float fd90 = energyBias + 2.0f * roughnessPercent * LdotH * LdotH;

    float lightScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotL, 5.0f);
    float viewScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotV, 5.0f);

    return diffuseColor * lightScatter * viewScatter * energyFactor;
}

//float D_GGX(float roughness, float NoH, const float3 NxH)
//{
//    float a = NoH * roughness;
//    float k = roughness / (dot(NxH, NxH) + a * a);
//    float d = k * k * (1.0 / PI);
//    return min(d, 65504.0);
//}

// GGX Specular D (normal distribution)
// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float D_GGX(in float roughness2, in float NdotH)
{
    const float alpha = roughness2 * roughness2;
    const float NdotH2 = NdotH * NdotH; // NdotH2 = NdotH^2

    const float lower = (NdotH2 * (alpha - 1.0f)) + 1.0f;
    //const float lower = NdotH2 * alpha + (1.0f - NdotH2);
    return alpha / (PI * lower * lower);
}

// Shlick's approximation of Fresnel By Unity Engine - Specular_F_Fresnel_Shlick
float3 F_Shlick(in float3 specularColor, in float HdotV)
{
    float FC = pow(1.0f - HdotV, 5.0f);
    return specularColor + (float3(1.0f, 1.0f, 1.0f) - specularColor) * FC;
}

// Schlick-Smith specular G (visibility) By Unity Version
float G_Smith(float roughness2, float NdotV, float NdotL)
{
    float SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * roughness2) + roughness2);
    float SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * roughness2) + roughness2);

    return 0.5f / max(SmithV + SmithL, 1e-5f);
}

float GGX_Geometry(float cosThetaN, float roughness4)
{
    float cosTheta_sqr = saturate(cosThetaN * cosThetaN);
    float tan2 = (1.0f - cosTheta_sqr) / cosTheta_sqr;

    return 2.0f / (1.0f + sqrt(1.0f + roughness4 * tan2));
}

float G_GGX(in float roughness2, in float NdotV, in float NdotL)
{
    float alpha = roughness2 * roughness2;

    return GGX_Geometry(NdotV, alpha) * GGX_Geometry(NdotL, alpha);
}

float G_SmithShlick(in float roughness2, in float NdotV, in float NdotL)
{
    float r = sqrt(roughness2) + 1.0f;
    float k = (r * r) / 8.0f;

    float SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float SmithL = NdotL / (NdotL * (1.0 - k) + k);

    return SmithV * SmithL;
}


float3 BRDF(in float roughness2, in float metallic, in float3 diffuseColor, in float3 specularColor, in float NdotH, in float NdotV, in float NdotL, in float HdotV)
{
    // Distribution & Geometry & Fresnel
    /*
    */
    float D = D_GGX(roughness2, NdotH); // �̼��� ���� �Լ�
    float G = G_GGX(roughness2, NdotV, NdotL); // ���� ���� �Լ�
    float3 F = F_Shlick(specularColor, HdotV); // ������ �Լ�

    /*float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - metallic;*/

    float3 kD = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);

    // Diffuse & Specular factors
    float denom = max(4.0f * NdotV * NdotL, 0.001f); // 0.001f just in case product is 0 , �Ϲ������� ����ŧ���� 4�� ������ ���� ������
    float3 specular_factor = saturate((D * F * G) / denom);
    float3 diffuse_factor = kD * diffuseColor;
    
    return (diffuse_factor + specular_factor) * NdotL;
}

// Unity�� BRDF Specular Term�� ������ ���ҽ��ϴ�.
	// HoV == Half dot View => Light dot Half�� ���� ���
float DirectBRDFSpecular(in float roughness2, in float3 NoH, in float3 HoV)
{
    const float roughness2MinusOne = roughness2 - 1.0f;
    // ������ ���� �����ֱ� ������.. ������ �� �� ����.
    const float normalizationTerm = sqrt(roughness2) * 4.0f + 2.0f;

    // �Ʒ��� Unity�� �ּ��Դϴ�..
    // �⺻������ GGX�� ���������..
    // GGX Distribution multiplied by combined approximation of Visibility and Fresnel
    // BRDFspec = (D * V * F) / 4.0
    // D = roughness^2 / ( NoH^2 * (roughness^2 - 1) + 1 )^2
    // V * F = 1.0 / ( LoH^2 * (roughness + 0.5) )
    // See "Optimizing PBR for Mobile" from Siggraph 2015 moving mobile graphics course
    // https://community.arm.com/events/1155

    // Final BRDFspec = roughness^2 / ( NoH^2 * (roughness^2 - 1) + 1 )^2 * (LoH^2 * (roughness + 0.5) * 4.0)
    // We further optimize a few light invariant terms
    // brdfData.normalizationTerm = (roughness + 0.5) * 4.0 rewritten as roughness * 4.0 + 2.0 to a fit a MAD.
    float d = NoH * NoH * roughness2MinusOne + 1.00001f;
    float d2 = d * d;

    float HoV2 = HoV * HoV;

    return roughness2 / (d2 * max(0.1f, HoV2) * normalizationTerm);
}


// Unity�� ��ҵ��� ������ ���ο� BRDF �Լ��Դϴ�.
float3 BRDF(in float roughness2, in float metallic, in float3 diffuseColor, in float3 specularColor, in float NdotH, in float NdotL, in float HdotV)
{
    // ���� ���� �̿��� Specular ���.
    float3 specular_factor = DirectBRDFSpecular(roughness2, NdotH, HdotV);
    
    // metallic�� �̿��� diffuse Factor ���.. in unity
    float oneMinusDielectricSpec = kDielectricSpec.a;
    float oneMinusReflectivity = oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
    float3 diffuse_factor = diffuseColor * oneMinusReflectivity;

    return (diffuse_factor + specular_factor * specularColor) * NdotL;
}


float3 PBR_DirectionalLight(
    in float3 V, in float3 N, in DirectionalLight light,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

    // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

    // Calculate Directional Light
    const float3 L = normalize(-light.direction);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = abs(dot(N, V)) + EPSILON;
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    // BRDF
    //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

    // Directional light
    light.color = SRGBToLinear(light.color);
    acc_color += light.color.rgb * light.power * shadow * brdf_factor;

    return acc_color;
}

float3 PBR_PointLight(
    in float3 V, in float3 N, in PointLight lights[POINT_LIGHT_COUNT], in uint lightCount, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    PointLight light;

    [unroll]
    for (uint i = 0; i < lightCount; i++)
    {
        light = lights[i];

        // Light vector (to light)
        float3 lightVec = light.position - position;
        float distance = length(lightVec);

        if (distance > light.range)
            continue;

        const float3 L = normalize(lightVec);
        const float3 H = normalize(V + L);

        // products
        const float NdotL = max(dot(N, L), EPSILON);
        const float NdotV = max(dot(N, V), EPSILON);
        const float NdotH = max(dot(N, H), EPSILON);
        const float HdotV = max(dot(H, V), EPSILON);

        // Attenuation

        /// ���� PBR Point Light Attenuation
        //float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.range));
        //float Attn = DistToLightNorm * DistToLightNorm;

        /// Unity Ŀ���� SRP ��α׿��� ������ Attenuation
        float distSqr = max(dot(lightVec, lightVec), 0.00001f);
        float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
        float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);
        float Attn = rangeAttn / distSqr;

        /// Unity URP ������ hlsl���� ������ Attenuation
        //float distSqr = max(dot(lightVec, lightVec), 0.00001f);
        //float rSqr = max((light.range * light.range), 0.00001f);
        //float lightAttn = rcp(distSqr);
        //float fadeDistance = 0.8f * 0.8f * rSqr;        
        //half smoothFactor = half(saturate((rSqr - distSqr) / (rSqr - fadeDistance)));
        //float Attn = lightAttn * smoothFactor;

        float3 radiance = Attn * light.power;

        // Burley roughness bias
        const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
        const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
        const float3 c_spec = lerp(F_ZERO, albedo, metallic) * ambientOcclusion;

        // BRDF
        //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
        float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

        // Point light
        light.color = SRGBToLinear(light.color);
        // ������ ���͸� ������ݴϴ�.
        acc_color += light.color.rgb * radiance * shadow * brdf_factor;
    }

    return acc_color;
}

float3 PBR_OnePointLight(in float3 V, in float3 N, in PointLight light, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Light vector (to light)
    float3 lightVec = light.position - position;
    float distance = length(lightVec);

    if (distance > light.range)
        return float3(0.f, 0.f, 0.f);

    const float3 L = normalize(lightVec);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = max(dot(N, V), EPSILON);
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    // Attenuation

    /// Unity Ŀ���� SRP ��α׿��� ������ Attenuation
    float distSqr = max(dot(lightVec, lightVec), 0.00001f);
    float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
    float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);
    float Attn = rangeAttn / distSqr;

    //float distSqr = max(dot(lightVec, lightVec), 0.00001f);
    //float rSqr = max((light.range * light.range), 0.00001f);
    //float lightAttn = rcp(distSqr);
    //float fadeDistance = 0.8f * 0.8f * rSqr;        
    //half smoothFactor = half(saturate((rSqr - distSqr) / (rSqr - fadeDistance)));
    //float Attn = lightAttn * smoothFactor;

    float3 radiance = Attn * light.power;

    // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

    // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

    // BRDF
    //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

    // Point light
    light.color = SRGBToLinear(light.color);
    // ������ ���͸� ������ݴϴ�.
    return light.color.rgb * radiance * shadow * brdf_factor;
}

float3 PBR_SpotLight(
    in float3 V, in float3 N, in SpotLight lights[SPOT_LIGHT_COUNT], in uint lightCount, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    SpotLight light;

    const float rad2Deg = (PI * 2.0f) / 360.f;

    [unroll]
    for (uint i = 0; i < lightCount; i++)
    {
        light = lights[i];

        float3 lightVec = light.position - position;
        float distance = length(lightVec);

        if (distance > light.range)
            continue;

        const float3 L = normalize(lightVec);
        const float3 H = normalize(V + L);

        // products
        const float NdotL = max(dot(N, L), EPSILON);
        const float NdotV = max(dot(N, V), EPSILON);
        const float NdotH = max(dot(N, H), EPSILON);
        const float HdotV = max(dot(H, V), EPSILON);

        /// ���� Cone �� �Ÿ� Attenuation
        // Cone attenuation
        /*float attStart = light.range;
        float attRange = cos(degrees(0.5f * light.angle) * attStart) - cos(degrees(0.5f * light.angle));
        attStart = cos(degrees(0.5f * light.angle));
        float cosAng = dot(-light.direction, L);
        float conAtt = saturate((cosAng - attStart) / attRange);
        conAtt *= conAtt;*/

        // Attenuation
        /*float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.range));
        float Attn = DistToLightNorm * DistToLightNorm;*/

        /// Unity Ŀ���� SRP ��α׿��� ������ Attenuation
        // �⺻������ �Ÿ� Attenuation�� Point�� ����.
			// �ű⿡ ���⼺ �� ���� Attenuation�� �������� ��..
        float distSqr = max(dot(lightVec, lightVec), 0.00001f);
        float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
        float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);

        // Angle ���� ���
			// ����(Unity Light�� innerSpotAngle)
        float innerCos = cos(rad2Deg * 0.5f * light.innerAngle);
			// ���� spotAngle(�ܰ�)
        float outerCos = cos(rad2Deg * 0.5f * light.outerAngle);
        float angleRangeInv = 1.f / max(innerCos - outerCos, 0.0001f);
        float angleRangeInv2 = -outerCos * angleRangeInv;

        // 0������ 90�� ������.. Spot Attenuation (���� �ڵ�� �ٸ��ٰ� ���� ����)
        float spotAttn = pow(saturate(dot(-light.direction.xyz, L) * angleRangeInv + angleRangeInv2), 2.0f);
        // ���� ���� �����غ���?

        float Attn = spotAttn * rangeAttn / distSqr;

        float3 radiance = Attn * light.power;

        // Burley roughness bias
        const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
        const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
        const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

        // BRDF
        //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
        float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);


        // unity ������ ����� ���� �ڷ���
        light.color = SRGBToLinear(light.color);
        acc_color += light.color.rgb * radiance * brdf_factor;
    }

    return acc_color;
}

float3 PBR_OneSpotLight(in float3 V, in float3 N, in SpotLight light, in float3 position,
	in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    const float rad2Deg = (PI * 2.0f) / 360.f;

    float3 lightVec = light.position - position;
    float distance = length(lightVec);

    if (distance > light.range)
        return float3(0.f, 0.f, 0.f);

    const float3 L = normalize(lightVec);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = max(dot(N, V), EPSILON);
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    /// Unity Ŀ���� SRP ��α׿��� ������ Attenuation
    // �⺻������ �Ÿ� Attenuation�� Point�� ����.
		// �ű⿡ ���⼺ �� ���� Attenuation�� �������� ��..
    float distSqr = max(dot(lightVec, lightVec), 0.00001f);
    float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
    float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);

        // Angle ���� ���
			// ����(Unity Light�� innerSpotAngle)
    float innerCos = cos(rad2Deg * 0.5f * light.innerAngle);
			// ���� spotAngle(�ܰ�)
    float outerCos = cos(rad2Deg * 0.5f * light.outerAngle);
    float angleRangeInv = 1.f / max(innerCos - outerCos, 0.0001f);
    float angleRangeInv2 = -outerCos * angleRangeInv;

    // 0������ 90�� ������.. Spot Attenuation (���� �ڵ�� �ٸ��ٰ� ���� ����)
    float spotAttn = pow(saturate(dot(-light.direction.xyz, L) * angleRangeInv + angleRangeInv2), 2.0f);

    float Attn = spotAttn * rangeAttn / distSqr;

    float3 radiance = Attn * light.power;

        // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

        // BRDF
    //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

        // unity ������ ����� ���� �ڷ���
    light.color = SRGBToLinear(light.color);
    return light.color.rgb * radiance * shadow * brdf_factor;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    // Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    //float3 T = tangentW;
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    // Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

// Shadow Factor ���
float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH, float4 textureInfo)
{
	// NDC ���� ���̰�
    float depth = shadowPosH.z;

    // �ؼ��� ũ�� => ������ۿ��� �޾ƿ´�.
    const float dx = textureInfo.z;
    const float dy = textureInfo.w;

    // ����
    float shadowFactor = 0.f;

    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        shadowFactor += shadowMap.SampleCmpLevelZero(samShadow, shadowPosH.xy + offsets[i], depth).r;
    }

    // ������� ��� ��ȯ
    return shadowFactor /= 9.0f;
}

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2DArray shadowMap, float4 shadowPosH, float4 textureInfo, int idx)
{
	// NDC ���� ���̰�
    float depth = shadowPosH.z;

    // �ؼ��� ũ�� => ������ۿ��� �޾ƿ´�.
    const float dx = textureInfo.z;
    const float dy = textureInfo.w;

    // ����
    float shadowFactor = 0.f;

    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        shadowFactor += shadowMap.SampleCmpLevelZero(samShadow, float3(shadowPosH.xy + offsets[i], idx), depth).r;
    }

    // ������� ��� ��ȯ
    return shadowFactor /= 9.0f;
}

float CalcDepthInShadow(const in float3 fragPos, float range)
{
    const float c1 = range / (range - 1.0f);
    const float c0 = -1.0f * range / (range - 1.0f);
    const float3 m = abs(fragPos).xyz;

    // ��� ���� ���� ���Ѱ�..?
    const float major = max(m.x, max(m.y, m.z));

    return (c1 * major + c0) / major;

}

// Point Light Shadow ���
float CalcPointShadowFactor(SamplerComparisonState samShadow, int index, TextureCubeArray cubeShadowMap, float4 shadowPosH, float4 textureInfo, float range)
{
    // ���̰�
    float depth = CalcDepthInShadow(shadowPosH.xyz, range);

    shadowPosH = normalize(shadowPosH);

    // �ؼ��� ũ�� => ������ۿ��� �޾ƿ´�.
    const float dx = textureInfo.z;
    const float dy = textureInfo.w;

     // ����
    float shadowFactor = 0.f;

    /*const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };*/

    shadowPosH.w = index;

    /*[unroll]
    for (int i = 0; i < 9; i++)
    {
        shadowFactor += cubeShadowMap.SampleCmpLevelZero(samShadow, shadowPosH, depth).r;
    }*/

    shadowFactor = cubeShadowMap.SampleCmpLevelZero(samShadow, shadowPosH, depth).r;
    
    return shadowFactor;
}

float3 SRGBToLinear(float3 color)
{
    float3 linearRGBLo = color / 12.92f;
    float3 linearRGBHi = pow((color + 0.055f) / 1.055f, 2.4f);
    float3 linearRGB = (color <= 0.04045f) ? linearRGBLo : linearRGBHi;
    return linearRGB;
}

float4 SRGBToLinear(float4 color)
{
    return float4(SRGBToLinear(color.rgb), color.a);
}
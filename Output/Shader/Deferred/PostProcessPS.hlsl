// Deferred ���� ���� ������ Pxiel�鿡 ���� PostProcessing�� �����ϴ� Pixel Shader �Դϴ�.

// �ȼ� ���̴��� ������ �� => QuadVS�� OutPut��
	// ������ ����ϴ� ���� Texture ���� ����� �ݴϴ�..
struct PostPixelIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// light ������ ��� ���� RenderTarget�� Texture
	// �ش� �ؽ��Ŀ��� ������ ��, ��ó���� ���ݴϴ�.
Texture2D EndRenderTarget : register(t0);

SamplerState Sampler : register(s0);

// Unity�� Neutral Tonemapping �Լ��� �����Խ��ϴ�.
	// Hable, Heji, Frostbite�� ������ ����ϴ� �� �����ϴ�.
// Input ���� Linear RGB���� �ϹǷ�, light Pass�� ������ ���� ���ڵ��� �������ݴϴ�.
// Unity�� real�� float�� ������־����ϴ�.
float3 NeutralCurve(float3 x, float a, float b, float c, float d, float e, float f)
{
    return ((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}

float3 NeutralTonemap(float3 pixelColor)
{
	// ToneMap ��ġ��
    const float a = 0.2f;
    const float b = 0.29f;
    const float c = 0.24f;
    const float d = 0.272f;
    const float e = 0.02f;
    const float f = 0.3f;
    const float whiteLevel = 5.3f;
    const float whiteClip = 1.0f;

    float3 whiteScale = (1.0f).xxx / NeutralCurve(whiteLevel, a, b, c, d, e, f);
    pixelColor = NeutralCurve(pixelColor * whiteScale, a, b, c, d, e, f);
    pixelColor *= whiteScale;

    pixelColor /= whiteClip.xxx;

    return pixelColor;
}

// Unity�� Vignetting �Լ�.
float3 Vignette(float3 cin, float2 uv, float intensity, float smoothness)
{
	// center 0.5, color black �����Դϴ�..
    float2 center = float2(0.5f, 0.5f);
    // ������..(�ܰ��� ����.)
    float3 color = float3(0.f, 0.f, 0.f);
    float2 dist = abs(uv - center) * intensity;

    float vfactor = pow(saturate(1.0f - dot(dist, dist)), smoothness);

    return cin * lerp(color, (1.0f).xxx, vfactor);
}


// Linear Space ���� Gamma Space��
// hlsli�� �ִ� �Լ��� ã�� ���ؼ� �����..
float3 LinearToSRGB(float3 color)
{
    float3 sRGBLo = color * 12.92f;
    float3 sRGBHi = (pow(color, 1 / 2.4f) * 1.055f) - 0.055f;
    float3 sRGB = (color <= 0.0031308f) ? sRGBLo : sRGBHi;

    return sRGB;
}

float4 LinearToSRGB(float4 color)
{
    return float4(LinearToSRGB(color.rgb), color.a);
}


// ���ؽ� ���̴�
float4 main(PostPixelIn ppin) : SV_Target
{
    // ��ó���� �ޱ� ���� ����
    float3 retColor = EndRenderTarget.Sample(Sampler, ppin.Tex).xyz;


    // ToneMapping�� ���ݴϴ�.
#ifdef UseToneMapping
    retColor = NeutralTonemap(retColor);
    saturate(retColor);
#endif

#ifdef UseVignette
    // ����� �⺻���� �������ݴϴ�. �ʿ��ϸ� cBuffer�� �������ݴϴ�.
    retColor = Vignette(retColor, ppin.Tex, 0.25f, 0.4f);
#endif

    retColor = LinearToSRGB(retColor);

	return float4(retColor, 1.0f);
}
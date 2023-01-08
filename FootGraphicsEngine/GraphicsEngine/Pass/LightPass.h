#pragma once
#include "PassBase.h"
#include "Util/LightHelper.h"

namespace GraphicsEngineSpace
{
	// ���̴��� ����� light buffer
	class VertexShader;
	class PixelShader;
	class RenderTargetTexture;

	// Screen Render Target => Depth�� Viewport�� ������ �ִ�.
	class RenderTargetDepth;

	class LightPass : public PassBase
	{
	public:
		std::shared_ptr<RenderTargetDepth> lightingRTV;

	private:
		std::shared_ptr<VertexShader> quadVS;
		std::shared_ptr<PixelShader> PBRLightPS;
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		bool isShadow = false;

		// �ϴ��� ���� EyePos�� ���⼭ ������ �־�� -> ���Ŀ� ���� ����
		SimpleMath::Vector3 eyePos;

		// ���� ������Ʈ �Ǿ���Ѵ�..? �ѹ� ������ ����
		PBR::DirectionalLight directionalLight;
		std::vector<PBR::PointLight> pointLights;
		std::vector<PBR::SpotLight> spotLights;

	public:
		void Start(const std::shared_ptr<RenderTargetDepth>& mainRTV) override;
		void Release() override;

		void OnResize(int width, int height) override;

		void RenderStart();

		void Render(const std::vector<std::shared_ptr<RenderTargetTexture>>& renderTargets);

		void EndRender();

		void UpdateEyePos(SimpleMath::Vector3 eyePos) { this->eyePos = eyePos; }

		void SetDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj);

		void SetPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, SimpleMath::Matrix lightViewProj);

		void SetSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, float halfAngle, float range, SimpleMath::Matrix lightViewProj);
	};
}
#pragma once
#include "PassBase.h"
#include "Util/LightHelper.h"

namespace GraphicsEngineSpace
{
	// �н��� �ʿ��� ���� Ŭ���� ���漱��
	class VertexShader;
	class GeometryShader;
	class RenderTargetTexture;
	class DepthStencilResource;
	class IDXObject;

	// Shadow Map Texture�� ����� ���� �н�.
		// �����δ� RenderTarget�� �������ִ� ����
		// ���ϰ� �ִ� DepthStencilShader�� Shader Resource Texture�� ������ִ� ������ �Ѵ�.
	class ShadowPass : public PassBase
	{
		// ����Ÿ���� ���� �������� �ʴ´�. => ���� ���� ������ mainDSV
	private:
		std::shared_ptr<DepthStencilResource> mainDSV;
		// Directional Light�� ����� DSV
		std::shared_ptr<DepthStencilResource> dirShadowDSV;
		// ť��� �������� ������� ������ ť���.
		std::shared_ptr<DepthStencilResource> pointShadowDSV;
		// SpotLight�� ����� DSV
		std::shared_ptr<DepthStencilResource> spotShadowDSV;

		std::shared_ptr<VertexShader> shadowStaticVS;
		std::shared_ptr<VertexShader> shadowSkinnedVS;
		std::shared_ptr<VertexShader> geoShadowStaticVS;
		std::shared_ptr<VertexShader> geoShadowSkinnedVS;
		std::shared_ptr<GeometryShader> shadowGS;

		// �ȼ����̴��� ���� �������� �ʴ´�..
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		SimpleMath::Matrix lightViewProj;

		__declspec(align(16)) struct cbLightMatrix
		{
			SimpleMath::Matrix lightViewProj[6];
		};

		cbLightMatrix lightMatrix;

		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};

		// cubeMap ������ �ߴ°��� ���� ����.
		bool isSetPointLightCnt = false;
		// spot Light ���� ����
		bool isSetSpotLightCnt = false;

	public:
		virtual void Start(const std::shared_ptr<RenderTargetTexture>& mainRTV, const std::shared_ptr<DepthStencilResource>& mainDSV) override;
		virtual void Release() override;

		virtual void OnResize(int width, int height) override;

		// ShadowCubeMap�� light ������ŭ �������ݴϴ�.
		void SetPointLightArray(int pointLightCnt);
		// Spot Light ���� ��ŭ ������ ���� �����մϴ�.
		void SetSpotLightArray(int spotLightCnt);

		// Shadow Render Target�� �����ص� ���·� -> �׸��ڸ� �帮�� ��� ������Ʈ�� �׷�����Ѵ�.
			// �� �������� �׸��ڸ� �帮�� ������Ʈ�� PBR Obj�̸�.. ���� �ش� ������Ʈ ���� ���� ���� ���ְų� �ؾ��� ���� �ִ�.
			// PBR Obj �ȿ� Shadow Render �Լ��� �ִ� �͵� ����� �� �ִ�..
		// RenderTarget ���� �����ϴ� ����
		void RenderStart();
		// ��ǻ� PBR Obj �ȿ��� ���ƾ� �� �� ����..
			// �ƴϸ� obj Queue�� �ްų�..
		void Render(std::shared_ptr<IDXObject> object);
		// ����� ���ҽ� �ʱ�ȭ
		void EndRender();

		// Point Light�� �Լ��� ���� ������
		void PointRenderStart(int idx);
		void PointRender(std::shared_ptr<IDXObject> object);

		// Spot Light �� ���� �Լ�
		void SpotRenderStart(int idx);

		void SetLightViewProj(SimpleMath::Matrix lightViewProj);
		void SetLightMatrix(std::vector<SimpleMath::Matrix> lightMatrix);
		void SetLightMatrix(PBR::PointLight pointLight);

		std::shared_ptr<DepthStencilResource> GetShadowDSV() { return dirShadowDSV; }
		std::shared_ptr<DepthStencilResource> GetPointShadowDSV() { return pointShadowDSV; }
		std::shared_ptr<DepthStencilResource> GetSpotShadowDSV() { return spotShadowDSV; }


		bool GetIsSetPointLightCnt() { return isSetPointLightCnt; }
		void ResetIsSetPointLightCnt();
		
		bool GetIsSetSpotLightCnt() { return isSetSpotLightCnt; }
		void ResetIsSetSpotLightCnt();
	};
}


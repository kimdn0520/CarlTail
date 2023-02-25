#pragma once

#include "PassBase.h"

namespace GraphicsEngineSpace
{
	class VertexShader;
	class PixelShader;
	class RenderTargetTexture;
	class DepthStencilResource;

	// ���� ������ ���
		// �׸��� ����(DSV) => LightPass�� RenderTarget�� Render
		// => �ش� Ÿ���� Texture�� �޾Ƽ� PostProcessing => �� ����� Back���ۿ� �׸�.

	/**
	 * \brief PostProcessing�� ���� �н��Դϴ�.
	 *
	 * Deferred Render ���� ��� ������ ���� Render Target�� �޾ƿ� �ȼ����� ó�����ִ� Pass �Դϴ�.
	 */
	class PostProcess : public PassBase
	{
	private:
		// ����� ����� RTV(main)
		std::shared_ptr<RenderTargetTexture> mainRTV;
		// �׸� �� ����� DSV (��ǻ� main)
		std::shared_ptr<DepthStencilResource> mainDSV;

		// ���� ���̴�
		std::shared_ptr<VertexShader> quadVS;
		std::shared_ptr<PixelShader> postProcessPS;

		// Reset�� ���� �׷��Ƚ� �ھ�
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		// ����� ToneMapping �ϳ����� ���߿� ����Ʈ ���μ��� ������ �þ�ٸ�
		// �߰������� Ʈ���ſ� bool ������ PixelShader ������ �߰��ؾ��մϴ�.

	public:
		virtual void Start(const std::shared_ptr<RenderTargetTexture>& mainRTV, const std::shared_ptr<DepthStencilResource>& mainDSV) override;
		virtual void Release() override;

		virtual void OnResize(int width, int height) override;

		void RenderStart();

		void Render(const std::shared_ptr<RenderTargetTexture>& renderTarget);

		void EndRender();

		void ImGuiRender();
	};

}

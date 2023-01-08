#pragma once

namespace GraphicsEngineSpace
{
	/**
	 * \brief ȭ�鿡 �׸��� �׸��� ���� RenderTargetView, DepthStencilView ���� ��Ƶ� Ŭ����.
	 *
	 * ���Ŀ� ��Ƽ ���� Ÿ�� ���� �ϸ鼭 ����� �߰� �ǰ�����,
	 * ����(2022.07.01)�� RenderTargetView�� DepthStencilView ���� �Ҿ������ �ʰ� �� �����ϴ� ���Ҹ� �Ѵ�.
	 *
	 * (2022.10.24 �߰�)
	 * �ش� Ŭ������ ȭ�鿡 �ֿ��ϰ� ǥ�� �Ǵ� ���� Ÿ���� �ǹ��ϸ�, �ٸ� ����Ÿ�ٰ� �ٸ��� �����ϰ�
	 * Depth StencilView�� ������.
	 *
	 * 2022.07.01 Dev.BlackFoot
	 */
	class RenderTargetDepth
	{
	private:
		// ���� ���ٽ� ����
		ComPtr<ID3D11Texture2D> depthStencilBuffer;

		// ���� ��
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		ComPtr<ID3D11DepthStencilView> depthStencilView;

		// ���� ���ٽ� ������Ʈ
		ComPtr<ID3D11DepthStencilState> depthStencilState;

		// Screen ViewPort
		// ��ũ������ ����ϴ� ����Ʈ�� DESC�� ���ص� screenViewport
			// CubeMap�� �׸��� ���� Viewport�� ���� ����������ϱ� ������..
		std::shared_ptr<D3D11_VIEWPORT> screenViewport;


	public:
		RenderTargetDepth();
		~RenderTargetDepth();

		void Finalize();

		// ���� ����..
			// �ܺο��� ���� �����ؾ� �ϱ� ������.. const�� ������ �ʴ´�..
			// �̰� �ȵǸ� ���ľ� ��

			// ���� �ȵǸ�.. �����͸� ���۷����� ��������..
			// �ƴϸ� ����̽��� �޾Ƽ� ���⼭ �����ϴ� ������ �غ���..(�̰� �� ���� ���� �ִ�.)
			// r-value�� ��� �ȴ�..
		ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return renderTargetView; }
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return depthStencilView; }
		ComPtr<ID3D11Texture2D> GetDepthStencilBuffer() { return depthStencilBuffer; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return depthStencilState; }
		std::shared_ptr<D3D11_VIEWPORT> GetViewport() { return screenViewport; }

		// ����̽��� �޾Ƽ� ���� Ÿ���� ���� ����� �ִ� ���� (����� �°�..)
			// ����� �޾ƿ�����.
		void CreateRenderTarget(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC, ComPtr<IDXGISwapChain> _swapChain,
			int _clientWidth, int _clientHeight, UINT _4xMSAAQuality);

		// OnResize ó���� ���� Ÿ�ٵ��� �����ִ� �Լ�.
		void DeleteImmediateRenderTarget();
	};
}

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
	 * (2023. 01. 12 ���� ����)
	 * �ش� Ŭ������ shadow Map �� ���ο� ���� ������ �ʿ��� �� ����� depthStencil View���� ������.
	 * ���̴� ���ҽ��� �߰��� SRV�� �߰��� ������ �Ѵ�.
	 *
	 * 2022.07.01 Dev.BlackFoot
	 */
	class DepthStencilResource
	{
	private:
		// ���� ���ٽ� ����(Resource�� ����� Texture)
		ComPtr<ID3D11Texture2D> depthStencilBuffer;

		// ���� ���ٽ� ��
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		std::vector<ComPtr<ID3D11DepthStencilView>> depthStencilArray;

		// ���� ���ٽ� ������Ʈ
		ComPtr<ID3D11DepthStencilState> depthStencilState;

		// shader�� �־��� ���ҽ�
		ComPtr<ID3D11ShaderResourceView> shaderResource;

		// Screen ViewPort
		// ��ũ������ ����ϴ� ����Ʈ�� DESC�� ���ص� screenViewport
			// CubeMap�� �׸��� ���� Viewport�� ���� ����������ϱ� ������..
		std::shared_ptr<D3D11_VIEWPORT> screenViewport;


	public:
		DepthStencilResource();
		~DepthStencilResource();

		void Finalize();

		// ���� ����..
			// �ܺο��� ���� �����ؾ� �ϱ� ������.. const�� ������ �ʴ´�..
			// �̰� �ȵǸ� ���ľ� ��

			// ���� �ȵǸ�.. �����͸� ���۷����� ��������..
			// �ƴϸ� ����̽��� �޾Ƽ� ���⼭ �����ϴ� ������ �غ���..(�̰� �� ���� ���� �ִ�.)
			// r-value�� ��� �ȴ�..
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return depthStencilView; }
		ComPtr<ID3D11Texture2D> GetDepthStencilBuffer() { return depthStencilBuffer; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return depthStencilState; }
		std::vector<ComPtr<ID3D11DepthStencilView>> GetDepthStencilArray() { return depthStencilArray; }
		ComPtr<ID3D11ShaderResourceView> GetShaderResource() { return shaderResource; }
		std::shared_ptr<D3D11_VIEWPORT> GetViewport() { return screenViewport; }

		// ����̽��� �޾Ƽ� ���� Ÿ���� ���� ����� �ִ� ���� (����� �°�..)
			// ����� �޾ƿ�����.
		void CreateDepthStencil(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC,
			int _clientWidth, int _clientHeight);

		// Point Light�� ���� Cube Map Resource�� ������ݴϴ�..
		void CreateDepthStencilCube(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC,
			int _clientWidth, int _clientHeight, int cubeMapCnt);

		// Spot Light�� ���� Texture2D Array Resource �õ�
		void CreateDepthStencilArray(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC,
			int _clientWidth, int _clientHeight, int arrayCnt);


		// OnResize ó���� ���� Ÿ�ٵ��� �����ִ� �Լ�.
		void DeleteImmediateDepthStencil();
	};
}

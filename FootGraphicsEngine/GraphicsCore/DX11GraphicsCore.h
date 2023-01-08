#pragma once

namespace GraphicsEngineSpace
{
	/**
	* \brief Device, DeviceContext, SwapChain, RasterizerState ���� �����ϴ� �ھ� Ŭ����
	*
	* ���������� ���� �ʱ�ȭ�� �����Ѵ�, Getter�� ������ Device ���� ������ ��ȯ�� �� �ִ�.
	* Graphics ���ο����� ����� ���̱� ������ ���� Interface�� ���� �ʴ´�.
	*
	* �� �� �پ��� ������ ����ϱ� ���� �̱��� Ŭ������ ����� �־���.
	*
	* 2022.10.04(ȭ) Dev.BlackFoot
	*/
	class DX11GraphicsCore
	{
	private:
		// �̱��濡 ����ϱ� ���� �ν��Ͻ�
		static std::shared_ptr<DX11GraphicsCore> instance;

		// ������ �ִ� ���� Device���� ������..
		ComPtr<ID3D11Device> D3DDevice;					// Device
		ComPtr<ID3D11DeviceContext> D3DImmediateContext;	// DC
		ComPtr<IDXGISwapChain> swapChain;				// SwapChain

		// ��Ƽ �ٸ������ ���� ����
		UINT _4xMSAAQuality;
		bool enable4xMSAA;

		// ���� ��ũ���� width�� Height�� ������ �ִ´�.
		int screenWidth;
		int screenHeight;

	public:
		DX11GraphicsCore();
		~DX11GraphicsCore();

		static std::shared_ptr<DX11GraphicsCore> GetInstance();

		// �Ʒ��� ��� ������ �ʱ�ȭ ���ִ� ������ �ʿ��ϴ�.
		// ���� �������� ������ �ִ� ���� �ʱ�ȭ �Լ��� ����� �ű��.
		// ���������� �Լ��� ȣ�� �Ǹ� ����� �Ѱܼ� ó���ϴ� �������.
		bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight);
		void Finalize();

		// ������ �ش�Ǵ� ���� �Լ��� ����
		void ResetView(ComPtr<ID3D11RenderTargetView> _nowRT, ComPtr<ID3D11DepthStencilView> _nowDSV, const FLOAT color[4]);
		void ResetRS();
		void PresentSwapChain();

		// ������? ���͸� ����� ����.
			// ���ʹ� ���� �ʿ䰡 ����..
			// const�� �ϴ°� �����Ϸ���..
		ComPtr<ID3D11Device> GetDevice() const { return D3DDevice; }
		ComPtr<ID3D11DeviceContext> GetImmediateDC() const { return D3DImmediateContext; }
		ComPtr<IDXGISwapChain> GetSwapChain() const { return swapChain; }

		UINT GetMSAAQuality() const { return _4xMSAAQuality; }

		int GetScreenWidth() const { return screenWidth; }
		int GetScreenHeight() const { return screenHeight; }

		void SetScreenWidth(int screenWidth) { this->screenWidth = screenWidth; }
		void SetScreenHeight(int screenHeight) { this->screenHeight = screenHeight; }
	};

}

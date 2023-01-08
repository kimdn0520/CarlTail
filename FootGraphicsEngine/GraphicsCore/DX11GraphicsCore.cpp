#include "GraphicsPch.h"
#include "DX11GraphicsCore.h"
#include "RasterizerState.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<DX11GraphicsCore> DX11GraphicsCore::instance = nullptr;

	DX11GraphicsCore::DX11GraphicsCore()
		: D3DDevice(nullptr)
		, D3DImmediateContext(nullptr)
		, swapChain(nullptr)
		, _4xMSAAQuality(0)
		, enable4xMSAA(false)
		, screenWidth(0)
		, screenHeight(0)
	{

	}

	DX11GraphicsCore::~DX11GraphicsCore()
	{
		if (D3DImmediateContext)
			D3DImmediateContext->ClearState();
	}

	std::shared_ptr<DX11GraphicsCore> DX11GraphicsCore::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<DX11GraphicsCore>();

		return instance;
	}

	bool DX11GraphicsCore::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
	{
		// _hWnd�� �����´�..
		// Device�� DC�� �����ô�..
		UINT _createDeviceFlags = 0;

		// ����׸�..
#if defined(DEBUG) || defined(_DEBUG)
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL _featureLevel;
		HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0,
			_createDeviceFlags, 0, 0, D3D11_SDK_VERSION,
			D3DDevice.GetAddressOf(), &_featureLevel, D3DImmediateContext.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}

		// ���� Ȯ��.
		// ���� Ȯ��. ���⼭ �ٸ��� ������ �迭�� �ϳ� �����ؼ� �߰��ؾ���.
		if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(_hWnd, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// 4X MSAA ǰ�� ���� ����
		HR(D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMSAAQuality));
		assert(_4xMSAAQuality > 0);

		// SwapChain ����.

		// DXGI_SWAP_CHAIN_DESC ����ü ����
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = _clientWidth;
		sd.BufferDesc.Height = _clientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		if (enable4xMSAA)
		{
			sd.SampleDesc.Count = 4;

			sd.SampleDesc.Quality = _4xMSAAQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = _hWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		IDXGIDevice* _dxgiDevice = 0;
		HR(D3DDevice->QueryInterface(__uuidof(IDXGIDevice),
			(void**)(&_dxgiDevice)));

		IDXGIAdapter* _dxgiAdapter = 0;
		HR(_dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
			(void**)&_dxgiAdapter));

		IDXGIFactory* _dxgiFactory = 0;
		HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
			(void**)&_dxgiFactory));

		HR(_dxgiFactory->CreateSwapChain(D3DDevice.Get(), &sd, swapChain.GetAddressOf()));

		ReleaseCOM(_dxgiDevice);
		ReleaseCOM(_dxgiAdapter);
		ReleaseCOM(_dxgiFactory);

		// ���� OnResize�� ��ģ ��..
			// ���⼭ Device, DC ���� ���� ���� �ϴ� �� ������.
			// �ϴ���.. ���� ���۸� �����ϴ� �������� swapChain�� Device �� ���� �ִ� �� �� ���Ǳ� �ϴµ�...
			// ������ ���⿡ ���� �並 ��� ��� �� �� ���� ���ΰ�.
			// �⺻������ ���⼭ ��� ���۵��� �����ϰ� �ȴ�..
				// �����غ��� ���⿡ �����ϴ� View���� ���������� ȭ�鿡 �׷��ִ� View���̱� ������ �׳� ��������� ������ �־..?
				// �������� ������ �� ����.
			// �ۿ��� ���ڷ� �޾Ƽ� onresize�� �ϴ� �����.. ������ �����ѵ� �ǹ̰� ���� �� ����..

		// ���� ������Ʈ ����
		// ���� ���� ������Ʈ�� �����ϴ� ������ ����������..
		RasterizerState::InitAllRS(D3DDevice);

		screenWidth = _clientWidth;
		screenHeight = _clientHeight;

		// ������� �ϸ� ����
		return true;
	}

	void DX11GraphicsCore::Finalize()
	{
		// ���� ģ���� ����.
		if (D3DImmediateContext)
			D3DImmediateContext->ClearState();

		SafeReset(instance);
	}

	/**
	* \brief ���� ������ �����ִ� �ܰ�.
	* \param _nowRT ���� ���� Ÿ�� ��
	* \param _nowDSV ���� ���� ���ٽ� ��
	*/
	void DX11GraphicsCore::ResetView(ComPtr<ID3D11RenderTargetView> _nowRT, ComPtr<ID3D11DepthStencilView> _nowDSV, const FLOAT color[4])
	{
		assert(D3DImmediateContext);
		assert(swapChain);

		// ����Ÿ�ٰ� ���ٽ� ���۸� �ʱ�ȭ�Ѵ�.
		D3DImmediateContext->ClearRenderTargetView(_nowRT.Get(), color);
		if(_nowDSV != nullptr)
			D3DImmediateContext->ClearDepthStencilView(_nowDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11GraphicsCore::ResetRS()
	{
		D3DImmediateContext->RSSetState(0);
	}

	void DX11GraphicsCore::PresentSwapChain()
	{
		assert(swapChain);

		HR(swapChain->Present(1, 0));
	}

}

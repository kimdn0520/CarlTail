#include "GraphicsPch.h"
#include "RenderTargetDepth.h"

namespace GraphicsEngineSpace
{
	RenderTargetDepth::RenderTargetDepth()
		: depthStencilBuffer(nullptr)
		, renderTargetView(nullptr)
		, depthStencilView(nullptr)
		, depthStencilState(nullptr)
		, screenViewport(nullptr)
	{
	}

	RenderTargetDepth::~RenderTargetDepth()
	{
	}

	void RenderTargetDepth::Finalize()
	{
		SafeReset(screenViewport)
	}

	// �並 �����ô��� r-value�� ��� �Ǿ��� ������ ��ġ���� �޾Ƽ� �������ִ� ������ �غ���..
		// ��Ȯ�ϰԴ� CreateRenderTarget �κ�.
	void RenderTargetDepth::CreateRenderTarget(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC, ComPtr<IDXGISwapChain> _swapChain, int _clientWidth, int _clientHeight, UINT _4xMSAAQuality)
	{
		// ���� �ִ� �Ű� ������ ����� �����Ǿ����� Ȯ��.
		assert(_device);
		assert(_immediateDC);
		assert(_swapChain);

		// ���� Ÿ�� �� ����
		HR(_swapChain->ResizeBuffers(1, _clientWidth, _clientHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
		ID3D11Texture2D* _backBuffer;
		_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&_backBuffer));

		if (_backBuffer != 0)
		{
			_device->CreateRenderTargetView(_backBuffer, 0, renderTargetView.GetAddressOf());
		}
		else
		{
			return;
		}
		ReleaseCOM(_backBuffer);

		// ���� ���ٽ� ����, �� ����
		D3D11_TEXTURE2D_DESC _depthStencilDesc;
		_depthStencilDesc.Width = _clientWidth;
		_depthStencilDesc.Height = _clientHeight;
		_depthStencilDesc.MipLevels = 1;
		_depthStencilDesc.ArraySize = 1;
		_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_depthStencilDesc.SampleDesc.Count = 1;
		_depthStencilDesc.SampleDesc.Quality = 0;

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&_depthStencilDesc, 0, depthStencilBuffer.GetAddressOf()));

		HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf()));

		// State ����
		D3D11_DEPTH_STENCIL_DESC descDSS = {};
		ZeroMemory(&descDSS, sizeof(D3D11_DEPTH_STENCIL_DESC));

		descDSS.DepthEnable = true;
		descDSS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		descDSS.StencilEnable = false;
		descDSS.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		descDSS.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		descDSS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&descDSS, depthStencilState.GetAddressOf()));

		SafeReset(screenViewport);

		screenViewport = std::make_shared<D3D11_VIEWPORT>();

		// Screen Viewport ����
		screenViewport->TopLeftX = 0.0f;
		screenViewport->TopLeftY = 0.0f;
		screenViewport->Width = static_cast<float>(_clientWidth);
		screenViewport->Height = static_cast<float>(_clientHeight);
		screenViewport->MinDepth = 0.0f;
		screenViewport->MaxDepth = 1.0f;
	}

	// ������ ���� Ÿ���� �����ִ� �Լ�.
	void RenderTargetDepth::DeleteImmediateRenderTarget()
	{
		// ��� ��ɻ����δ� Finalize�� �ٸ� �ٰ� ����..
			// ���Ŀ� ���ο� ��� ������ �������� �� �޶��� ����..
			// ���⼭�� ȭ���� ����� �� �ٲ��ִ� ���� ���� Ÿ�ٵ鸸 �����ش�.
		if (renderTargetView)
			renderTargetView->Release();
		if (depthStencilView)
			depthStencilView->Release();
		if (depthStencilBuffer)
			depthStencilBuffer->Release();
		if (depthStencilState)
			depthStencilState->Release();
	}

}

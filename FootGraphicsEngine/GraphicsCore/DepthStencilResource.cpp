#include "GraphicsPch.h"
#include "DepthStencilResource.h"

namespace GraphicsEngineSpace
{
	DepthStencilResource::DepthStencilResource()
		: depthStencilBuffer(nullptr)
		, depthStencilView(nullptr)
		, depthStencilState(nullptr)
		, screenViewport(nullptr)
	{
	}

	DepthStencilResource::~DepthStencilResource()
	{
	}

	void DepthStencilResource::Finalize()
	{
		SafeReset(screenViewport)
	}

	// �並 �����ô��� r-value�� ��� �Ǿ��� ������ ��ġ���� �޾Ƽ� �������ִ� ������ �غ���..
		// ��Ȯ�ϰԴ� CreateRenderTarget �κ�.
	void DepthStencilResource::CreateDepthStencil(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC, int _clientWidth, int _clientHeight)
	{
		// ���� �ִ� �Ű� ������ ����� �����Ǿ����� Ȯ��.
		assert(_device);
		assert(_immediateDC);

		// ���� ���ٽ� ����(�ؽ���) ����
		D3D11_TEXTURE2D_DESC _depthStencilDesc;
		_depthStencilDesc.Width = _clientWidth;
		_depthStencilDesc.Height = _clientHeight;
		_depthStencilDesc.MipLevels = 1;
		_depthStencilDesc.ArraySize = 1;
		// ���� �信 ����� Format�� Shader Resource���� ����� Format�� �ٸ��� ������ (�ؼ��� �ٸ��� ������)
			// TYPELESS�� �������ݴϴ�.
			// �ϴ� ����� ���̸� ����ϴ� �κ��� main Depth�� Shadow �ۿ� ���⿡ �ϵ� �ڵ�������
			// ���Ŀ� �ʿ䰡 ����� ��� ������ ������ �����ô�..
		_depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthStencilDesc.SampleDesc.Count = 1;
		_depthStencilDesc.SampleDesc.Quality = 0;

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		// Shadow Map������ Texture�� �̾Ƴ��� ���� ���ε� �÷��׸� �߰��մϴ�.
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&_depthStencilDesc, 0, depthStencilBuffer.GetAddressOf()));

		// ���� ���ٽ� �� ����
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));

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

		// shader Resource ����
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = _depthStencilDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		HR(_device->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, shaderResource.GetAddressOf()));

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

	void DepthStencilResource::CreateDepthStencilCube(ComPtr<ID3D11Device> _device,
		ComPtr<ID3D11DeviceContext> _immediateDC, int _clientWidth, int _clientHeight, int cubeMapCnt)
	{
		assert(_device);
		assert(_immediateDC);

		// ���� ���ٽ� ���۸� ť��� �������� ����� ����..
		D3D11_TEXTURE2D_DESC _depthStencilCubeDesc;
		_depthStencilCubeDesc.Width = _clientWidth;
		_depthStencilCubeDesc.Height = _clientHeight;
		_depthStencilCubeDesc.MipLevels = 1;

		if (cubeMapCnt != 0)
			_depthStencilCubeDesc.ArraySize = 6 * cubeMapCnt;
		else
			_depthStencilCubeDesc.ArraySize = 6;
		// ���� �信 ����� Format�� Shader Resource���� ����� Format�� �ٸ��� ������ (�ؼ��� �ٸ��� ������)
			// TYPELESS�� �������ݴϴ�.
			// �ϴ� ����� ���̸� ����ϴ� �κ��� main Depth�� Shadow �ۿ� ���⿡ �ϵ� �ڵ�������
			// ���Ŀ� �ʿ䰡 ����� ��� ������ ������ �����ô�..
		_depthStencilCubeDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthStencilCubeDesc.SampleDesc.Count = 1;
		_depthStencilCubeDesc.SampleDesc.Quality = 0;

		_depthStencilCubeDesc.Usage = D3D11_USAGE_DEFAULT;
		// Shadow Map������ Texture�� �̾Ƴ��� ���� ���ε� �÷��׸� �߰��մϴ�.
		_depthStencilCubeDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthStencilCubeDesc.CPUAccessFlags = 0;
		// �ش� �÷��׸� Texture Cube�� �����ϴ�..
		_depthStencilCubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HR(_device->CreateTexture2D(&_depthStencilCubeDesc, 0, depthStencilBuffer.GetAddressOf()));

		// ���� ���ٽ� �� ����
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		// �̺κ��� ť����̱� ������ Texture2DArray�� ������ݴϴ�.
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		// ť��� �����̱� ������ Texture2D Array�� �ش� ������ ��� �Է��� �ݴϴ�.
			// �׷��� ������ ������ �߻��մϴ�.
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.ArraySize = 6;
		if (cubeMapCnt != 0)
		{
			for (int i = 0; i < cubeMapCnt; i++)
			{

				dsvDesc.Texture2DArray.FirstArraySlice = i * 6;
				ComPtr<ID3D11DepthStencilView> tmpDepthStencil;

				HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, tmpDepthStencil.GetAddressOf()));

				depthStencilArray.push_back(tmpDepthStencil);
			}
		}
		else
		{
			dsvDesc.Texture2DArray.FirstArraySlice = 0;		

			HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));
		}

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

		// shader Resource ����
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		if (cubeMapCnt != 0)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			// ť��� �����̱� ������ Texture2D Array�� �ش� ������ ��� �Է��� �ݴϴ�.
			srvDesc.TextureCubeArray.First2DArrayFace = 0;
			srvDesc.TextureCubeArray.NumCubes = cubeMapCnt;
			srvDesc.TextureCubeArray.MipLevels = _depthStencilCubeDesc.MipLevels;
			srvDesc.TextureCubeArray.MostDetailedMip = 0;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.Texture2D.MipLevels = _depthStencilCubeDesc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
		}
		HR(_device->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, shaderResource.GetAddressOf()));

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

	void DepthStencilResource::CreateDepthStencilArray(ComPtr<ID3D11Device> _device,
		ComPtr<ID3D11DeviceContext> _immediateDC, int _clientWidth, int _clientHeight, int arrayCnt)
	{
		// 2D Texture�� Array�� �����ϸ鼭 Spot Light�� �ٲ� �� ����
		// Shader Resource View Array���� ���� ���� �������..?

		// Light Pass���� ���ҽ��� �ϳ��� ������ ���ָ� �� �� ����.
		// ���� �ִ� �Ű� ������ ����� �����Ǿ����� Ȯ��.
		assert(_device);
		assert(_immediateDC);

		// ���� ���ٽ� ����(�ؽ���) ����
		D3D11_TEXTURE2D_DESC _depthStencilDesc;
		_depthStencilDesc.Width = _clientWidth;
		_depthStencilDesc.Height = _clientHeight;
		_depthStencilDesc.MipLevels = 1;

		// �ش� �κ��� spotLight ������ �°� �������ݴϴ�.
		if (arrayCnt != 0)
			_depthStencilDesc.ArraySize = arrayCnt;
		else
			_depthStencilDesc.ArraySize = 1;
		// ���� �信 ����� Format�� Shader Resource���� ����� Format�� �ٸ��� ������ (�ؼ��� �ٸ��� ������)
			// TYPELESS�� �������ݴϴ�.
			// �ϴ� ����� ���̸� ����ϴ� �κ��� main Depth�� Shadow �ۿ� ���⿡ �ϵ� �ڵ�������
			// ���Ŀ� �ʿ䰡 ����� ��� ������ ������ �����ô�..
		_depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthStencilDesc.SampleDesc.Count = 1;
		_depthStencilDesc.SampleDesc.Quality = 0;

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		// Shadow Map������ Texture�� �̾Ƴ��� ���� ���ε� �÷��׸� �߰��մϴ�.
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&_depthStencilDesc, 0, depthStencilBuffer.GetAddressOf()));
		
		// ���� ���ٽ� �� ����
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		// ť����� �ƴ����� Array�� �����ô�..
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		// Array Size�� Array�� �̷�� Texture �ϳ��� ����..
		dsvDesc.Texture2DArray.ArraySize = 1;

		if(arrayCnt != 0)
		{
			for (int i = 0; i < arrayCnt; i++)
			{
				dsvDesc.Texture2DArray.FirstArraySlice = i;
				ComPtr<ID3D11DepthStencilView> tmpDepthStencil;

				HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, tmpDepthStencil.GetAddressOf()));

				depthStencilArray.push_back(tmpDepthStencil);
			}
		}
		else
		{
			// ���� ���� �ϳ��� �������ݴϴ�.
			dsvDesc.Texture2DArray.FirstArraySlice = 0;

			HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));
		}

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

		// shader Resource ����
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		// Array�� �����غ��ô�.
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		// MultiSampling�� ���� �ʴ´ٰ� �����մϴ�.
		srvDesc.Texture2DArray.MipLevels = _depthStencilDesc.MipLevels;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		if(arrayCnt != 0)
			srvDesc.Texture2DArray.ArraySize = arrayCnt;
		else
			srvDesc.Texture2DArray.ArraySize = 1;

		HR(_device->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, shaderResource.GetAddressOf()));

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
	void DepthStencilResource::DeleteImmediateDepthStencil()
	{
		// ��� ��ɻ����δ� Finalize�� �ٸ� �ٰ� ����..
			// ���Ŀ� ���ο� ��� ������ �������� �� �޶��� ����..
			// ���⼭�� ȭ���� ����� �� �ٲ��ִ� ���� ���� Ÿ�ٵ鸸 �����ش�.
		if (depthStencilView)
			depthStencilView = nullptr;
		if (depthStencilBuffer)
			depthStencilBuffer = nullptr;
		if (depthStencilState)
			depthStencilState = nullptr;

		if(shaderResource)
			shaderResource = nullptr;

		// ���� Array ����.
		depthStencilArray.clear();
	}

}

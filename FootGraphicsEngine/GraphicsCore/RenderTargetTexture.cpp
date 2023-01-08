#include "GraphicsPch.h"
#include "RenderTargetTexture.h"

namespace GraphicsEngineSpace
{
	RenderTargetTexture::RenderTargetTexture() :
		renderTexture(nullptr)
		, renderTarget(nullptr)
		, shaderResource(nullptr)
	{

	}

	RenderTargetTexture::~RenderTargetTexture()
	{

	}

	bool RenderTargetTexture::Init(ComPtr<ID3D11Device> device, int width, int height, UINT MSAAQuality)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT hr;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// �ش� �κп��� ���� ����Ÿ�ٰ� ������ ��ġ�� ���� ȭ���� ���������� �������.
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// �ؽ��� ����
		hr = device->CreateTexture2D(&textureDesc, 0, renderTexture.GetAddressOf());

		if (FAILED(hr) == true)
		{
			::MessageBoxA(nullptr, "Texture Create Failed!", nullptr, MB_OK);
			return false;
		}

		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateRenderTargetView(renderTexture.Get(), &rtvDesc, renderTarget.GetAddressOf());
		if (FAILED(hr) == true)
		{
			::MessageBoxA(nullptr, "RTV Create Failed!", nullptr, MB_OK);
			return false;
		}

		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		// �Ӹ� ������ ��� �ٸ��� �ٲ���� ���� �� �ϴ�..
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(renderTexture.Get(), &srvDesc, shaderResource.GetAddressOf());
		if(FAILED(hr) == true)
		{
			::MessageBoxA(nullptr, "SRV Create Failed!", nullptr, MB_OK);
			return false;
			
		}

		return true;
	}

	void RenderTargetTexture::Finalize()
	{
		
	}
}

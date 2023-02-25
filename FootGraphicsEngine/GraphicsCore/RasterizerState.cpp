#include "GraphicsPch.h"
#include "RasterizerState.h"


namespace GraphicsEngineSpace
{
	// ����ƽ ���� �ʱ�ȭ
	ComPtr<ID3D11RasterizerState> RasterizerState::wireFrameRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::solidRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::pointDepthRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::spotDepthRS = nullptr;

	bool RasterizerState::InitAllRS(ComPtr<ID3D11Device> pDevice)
	{
		// ���� ������ �۾��̱⿡ ����� ������ ���� ����.
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;

		HR(pDevice->CreateRasterizerState(&rsDesc, wireFrameRS.GetAddressOf()));

		rsDesc.FillMode = D3D11_FILL_SOLID;
		HR(pDevice->CreateRasterizerState(&rsDesc, solidRS.GetAddressOf()));

		// �����츦 ���� depth RS
		D3D11_RASTERIZER_DESC depthRsDesc;
		ZeroMemory(&depthRsDesc, sizeof(D3D11_RASTERIZER_DESC));
		depthRsDesc.FillMode = D3D11_FILL_SOLID;
		depthRsDesc.CullMode = D3D11_CULL_NONE;
		depthRsDesc.FrontCounterClockwise = false;
		depthRsDesc.DepthClipEnable = true;
		// ��� Ư�� ���� ���� => ����� �ϴ� ��å�� �������� �۾��غ���.
		// ������ ����ġ
		depthRsDesc.DepthBias = 1000;
		// ���Ǵ� �ִ� ���� ����ġ. ���� �� ���� ������
			// �ش� ����� �ʹ� ������ �׸��� ���帧�� �ε巯 ����.
		depthRsDesc.DepthBiasClamp = 0.0f;
		// �ٰ����� ���� ����ġ.
		depthRsDesc.SlopeScaledDepthBias = 1.0f;

		HR(pDevice->CreateRasterizerState(&depthRsDesc, pointDepthRS.GetAddressOf()));

		// ������ ����ġ
		depthRsDesc.DepthBias = 100000;
		// ���Ǵ� �ִ� ���� ����ġ. ���� �� ���� ������
			// �ش� ����� �ʹ� ������ �׸��� ���帧�� �ε巯 ����.
		depthRsDesc.DepthBiasClamp = 0.0f;
		// �ٰ����� ���� ����ġ.
		depthRsDesc.SlopeScaledDepthBias = 1.0f;
		HR(pDevice->CreateRasterizerState(&depthRsDesc, spotDepthRS.GetAddressOf()));


		return true;
	}

	void RasterizerState::DestroyAll()
	{
		
	}
}

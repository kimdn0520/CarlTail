#include "GraphicsPch.h"
#include "PostProcess.h"

#include "GraphicsCore/DX11GraphicsCore.h"
#include "GraphicsCore/RenderTargetTexture.h"
#include "GraphicsCore/DepthStencilResource.h"

#include "Object/Shader/VertexShader.h"
#include "Object/Shader/PixelShader.h"

#include "Manager/BufferManager.h"
#include "Manager/ImGUIManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SamplerManager.h"

#include "Object/Buffer/BufferBase.h"

#include "Util/VertexStruct.h"

namespace GraphicsEngineSpace
{
	void PostProcess::Start(const std::shared_ptr<RenderTargetTexture>& mainRTV,
		const std::shared_ptr<DepthStencilResource>& mainDSV)
	{
		quadVS = std::dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader("QuadVS"));

		postProcessPS = std::dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader("PostProcessAllPS"));

		this->mainRTV = mainRTV;
		this->mainDSV = mainDSV;

		graphicsCore = DX11GraphicsCore::GetInstance();
	}

	void PostProcess::Release()
	{
		SafeReset(postProcessPS);
	}

	void PostProcess::OnResize(int width, int height)
	{
		// �Ѵ� main�� ����ϱ� ������ �ǹ̰� �����ϴ�..
	}

	void PostProcess::RenderStart()
	{
		// �׸� �κ��� ����ϰ� ���ݴϴ�..
		graphicsCore->ResetRenderTargetView(mainRTV->GetRenderTargetView(), Colors::AliceBlue);
		graphicsCore->ResetDepthStencilView(mainDSV->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		D3DDeviceContext->OMSetRenderTargets(1, mainRTV->GetRenderTargetView().GetAddressOf(), mainDSV->GetDepthStencilView().Get());
	}

	// ��� ������ �Ϸ��(Deferred�� lighting������� �Ϸ��) RenderTarget�� ���;��մϴ�.
	void PostProcess::Render(const std::shared_ptr<RenderTargetTexture>& renderTarget)
	{
		RenderStart();

		quadVS->SetUpShader();
		postProcessPS->SetUpShader();

		// ���� RenderTarget�� Texture�� ���ɴϴ�.
		D3DDeviceContext->PSSetShaderResources(0, 1, renderTarget->GetShaderResourceView().GetAddressOf());
		D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

		// ScreenMesh�� �ҷ��ͼ� ȭ�鿡 �׷��ش�.
		auto screenMesh = ResourceManager::GetInstance()->basicMeshes["ScreenMesh"];
		D3DDeviceContext->RSSetState(screenMesh->GetRasterState().Get());

		UINT stride = sizeof(VertexStruct::TexVertex);
		UINT offset = 0;

		D3DDeviceContext->IASetPrimitiveTopology(screenMesh->GetPrimitiveTopology());
		D3DDeviceContext->IASetVertexBuffers(0, 1, screenMesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(screenMesh->indexBuffers[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		D3DDeviceContext->DrawIndexed(screenMesh->indexBuffersSize[0], 0, 0);

		EndRender();
	}

	void PostProcess::EndRender()
	{
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

		D3DDeviceContext->PSSetShaderResources(0, 1, nullSRV);
	}

	void PostProcess::ImGuiRender()
	{
		static bool isVignette = true;

		auto imgui = ImGUIManager::GetInstance();
		auto shaderManager = ShaderManager::GetInstance();
		
		if(imgui->Begin("Check Vignette"))
		{
			bool checkVignette = isVignette;

			imgui->CheckBox("Vignette", &checkVignette);

			isVignette = checkVignette;

			if(isVignette == true)
				postProcessPS = std::dynamic_pointer_cast<PixelShader>(shaderManager->GetShader("PostProcessAllPS"));
			else
				postProcessPS = std::dynamic_pointer_cast<PixelShader>(shaderManager->GetShader("PostToneMappingPS"));

		}
		imgui->End();
	}

}


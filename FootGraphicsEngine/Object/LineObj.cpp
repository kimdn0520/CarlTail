#include "GraphicsPch.h"
#include "LineObj.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/ResourceManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"

#include "GraphicsEngine/Renderer.h"

namespace GraphicsEngineSpace
{
	LineObj::LineObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, world()
		, view()
		, proj()
		, hasColor(false)
		, lineColor(DirectX::Colors::LimeGreen)
	{
	}

	LineObj::~LineObj()
	{
		for (auto objRes : meshResources)
		{
			SafeReset(objRes)
		}
	}

	bool LineObj::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// ���� Ȯ��
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		return true;
	}

	void LineObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// ��� �ֱ�
		world = _world;
		view = _view;
		proj = _proj;
	}


	void LineObj::Render()
	{
		// �⺻������ 0�� �ε����� ������ ����Ѵ� ����..
		assert(D3DDeviceContext);

		std::shared_ptr<Mesh> mesh = meshResources[0]->mesh;

		if (hasColor == true)
		{
			// ���� �⺻���� ��ǲ ���̾ƿ��� �����ɽô�.
			// ���� �Լ��� inputlayout ������ �ֽ��ϴ�.
			ShaderManager::GetInstance()->GetShader("BasicColorVS")->SetUpShader();
			// pixel ���̴�
			ShaderManager::GetInstance()->GetShader("BasicColorPS")->SetUpShader();

			// ���ؽ� ����, �ε��� ���� ����
			UINT stride = sizeof(VertexStruct::ColorVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		}
		else
		{
			ShaderManager::GetInstance()->GetShader("LineVS")->SetUpShader();
			// pixel ���̴�
			ShaderManager::GetInstance()->GetShader("LinePS")->SetUpShader();

			// pixel Shader�� Color Buffer�� �������ش�.
			BufferManager::GetInstance()->GetBuffer("ColorCB")->SetUpBuffer(1, &lineColor, ShaderType::PIXEL);

			// ���ؽ� ����, �ε��� ���� ����
			UINT stride = sizeof(VertexStruct::PosVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		}
		// ���� ������ �� ������ ���� -> ���� ���۰� ���� ����.
		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

		D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		// ��� ���� ����.
		// �����ʹ� constBuffer�� �������� ����ü�� ��������.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		// �ش� ���� �׸���.
		D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[0], 0, 0);
	}

	void LineObj::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.push_back(objResources);
	}

	std::string LineObj::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> LineObj::GetMeshResources()
	{
		return meshResources;
	}
}

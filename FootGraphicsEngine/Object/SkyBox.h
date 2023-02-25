#pragma once
#include "Interface/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief SkyBox ������Ʈ�Դϴ�.
	 *
	 * View Proj�� ���� �����̷��� ���� ������Ʈ�� ������ �ϱ� ������ ��������ϴ�.
	 */
	class SkyBox : public IDXObject
	{
		// ��� ���� ���
		// ����̽��� ������ ����.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// ���� ������ ���Ե� ���ҽ�
		std::vector<std::shared_ptr<MeshResources>> meshResources;

		// ĳ�̿� ���ҽ� �Ŵ��� ����
		std::shared_ptr<ResourceManager> resourceManager;

		// TM ��� => Ʈ���� ��..?
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// �ȿ� ���� cb
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		GRAPHICSENGINE_DECLSPEC SkyBox();
		~SkyBox() override;

		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) override;
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		virtual void PreRender(float tick) override {}
		virtual void Render() override;
		virtual void SetMeshResources(std::shared_ptr<MeshResources> objResources) override;
		virtual std::string GetObjName() override;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;

		virtual const SimpleMath::Matrix& GetWorld() override { return world; }
		virtual const SimpleMath::Matrix& GetView() override { return view; }
		virtual const SimpleMath::Matrix& GetProj() override { return proj; }
	};

}
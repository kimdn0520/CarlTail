#pragma once
#include "Interface/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief Grid, Axis��, ��ġ�� �÷� ���� ������ �⺻���� Pos ColorObj�� ���� ������Ʈ Ŭ����
	*
	* 2022.07.13(��) Dev.BlackFoot
	*/
	class LineObj : public IDXObject
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

		// ���ؽ��� Color ������ ����ִ����� ���� ����.
		bool hasColor;
		// Line�� �� �� ������ ��Ƶδ� ����.
		SimpleMath::Color lineColor;

		// �ȿ� ���� cb
		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		// �����ڴ� Export
		GRAPHICSENGINE_DECLSPEC LineObj();
		~LineObj() override;

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

		// dynamic ĳ������ �̿��Ͽ� color�� �������ش�.
		void SetHasColor(bool hasColor) { this->hasColor = hasColor; }
	};
}

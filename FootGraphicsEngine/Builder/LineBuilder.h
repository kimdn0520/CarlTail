#pragma once
#include "Interface/IBuilder.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief PosColor ������Ʈ���� ������ִ� PosColor Builder
	*
	* 2022.07.13(��) Dev.BlackFoot
	*/
	class LineBuilder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
			// ��� �Լ��� �������� �Ǹ鼭 ���� �κп��� ����ϴ� ���� ��� ������ �ʿ�������.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// ������� ���ҽ� �Ŵ���.
		std::shared_ptr<ResourceManager> resourceManager;
		

	public:
		LineBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{}
		~LineBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		//std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::string textureName, std::wstring path) override;
		std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;
		void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		// �ϵ��ڵ����� �־��� �׸���, ��
		std::shared_ptr<MeshResources> BuildGridResources(std::shared_ptr<MeshResources> _objRes);
		std::shared_ptr<MeshResources> BuildAxisResources(std::shared_ptr<MeshResources> _objRes);

	};

}

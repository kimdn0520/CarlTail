#pragma once
#include "Interface/IBuilder.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief SkyBox�� ������ִ� Builder
	 */
	class SkyBoxBuilder : public IBuilder
	{
		// ���������� device;
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// ������� ���ҽ� �Ŵ���
		std::shared_ptr<ResourceManager> resourceManager;

	public:
		SkyBoxBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{
			
		}
		~SkyBoxBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;
		void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		std::shared_ptr<MeshResources> BuildSkyBoxResources(std::shared_ptr<MeshResources> _objRes);

	};
}

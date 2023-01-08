#pragma once
#include "Interface/IBuilder.h"

#include "FBXBinaryLayout.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief PBR ���̴��� �̿��ؼ� �� ������ ó���ϴ� PBR object�� ���� ���� �Դϴ�.
	 */
	class PBRBuilder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		std::shared_ptr<ResourceManager> resourceManager;

	public:
		PBRBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{
			
		}
		~PBRBuilder() override;

		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		virtual std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;

		virtual void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		// ��ü ���˿��� ������ ���� mesh�� Resources ������ �߰����ݴϴ�.
		void BuildStaticMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);
		void BuildSkinnedMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);

	};

}
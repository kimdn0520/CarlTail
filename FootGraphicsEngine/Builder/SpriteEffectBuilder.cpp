#include "GraphicsPch.h"
#include "SpriteEffectBuilder.h"

#include "Interface/IDXObject.h"
#include "Manager/ResourceManager.h"
#include "Object/SpriteEffect.h"
#include "Resources/RenderingMaterial.h"

using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> SpriteEffectBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		if(std::dynamic_pointer_cast<SpriteEffect>(pDXObj) == nullptr)
			return nullptr;

		BuildGeometry(pDXObj, objectName);

		return pDXObj;
	}

	std::shared_ptr<MeshResources> SpriteEffectBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject,
		std::string objectPath)
	{
		std::shared_ptr<MeshResources> tmpRes = std::make_shared<MeshResources>();

		tmpRes->ObjName = objectPath;

		// ������Ʈ �̸��� �Ǻ��մϴ�.
		std::string extension = objectPath;
		int _extensionIdx = objectPath.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectPath.substr(_extensionIdx);
		}

		if (extension == ".model")
		{
			// ���� �ҷ����� �ϴ� path.. �ӽ�
			std::ifstream ifs("BinaryFile/Model/" + objectPath, std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive ia(buffer);

			FBXBinaryData::ModelData modelData;

			ia >> modelData;

			tmpRes.reset();

			BuildBinaryEffect(DXObject, modelData);
		}
		else
		{
			BuildDefaultEffect(DXObject, tmpRes);
			return tmpRes;
		}
	}

	void SpriteEffectBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		resourceManager = ResourceManager::GetInstance();
	}

	void SpriteEffectBuilder::BuildDefaultEffect(std::shared_ptr<IDXObject> DXObject, std::shared_ptr<MeshResources> tmpResource)
	{
		// �ϴ��� �ϵ��ڵ����� �׽�Ʈ�� obj�� �ϳ� �����ϴ�.
			// TODO �� �κ��� ���Ŀ� Effect Mesh�� �����ϸ� �������־�� �մϴ�.
		// Plane Effect ����
		std::vector<VertexStruct::TexVertex> vertices(6);

		// �� �ϴ�
		vertices[0].Pos = Vector3(-3.f, 0.f, -1.f);
		vertices[0].Tex = Vector2(0.f, 1.f);

		// �� ���
		vertices[1].Pos = Vector3(-3.f, 0.f, 1.f);
		vertices[1].Tex = Vector2(0.f, 0.f);

		// �� ���
		vertices[2].Pos = Vector3(3.f, 0.f, 1.f);
		vertices[2].Tex = Vector2(1.f, 0.f);

		// �� �ϴ�
		vertices[3].Pos = Vector3(-3.f, 0.f, -1.f);
		vertices[3].Tex = Vector2(0.f, 1.f);

		// �� ���
		vertices[4].Pos = Vector3(3.f, 0.f, 1.f);
		vertices[4].Tex = Vector2(1.f, 0.f);

		// �� �ϴ�
		vertices[5].Pos = Vector3(3.f, 0.f, -1.f);
		vertices[5].Tex = Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		auto mesh = resourceManager->CreateMesh(
			vertices,
			indices,
			"EffectMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		tmpResource->mesh = mesh;
		tmpResource->setMesh = true;

		// �ؽ��� �ϳ� ����.
		auto mat = std::make_shared<RenderingMaterial>();

		auto albedoTex = resourceManager->LoadTexture(L"Resources/Effect/EffectTest.png");
		mat->albedoTex = albedoTex;

		tmpResource->material.emplace_back(mat);

		tmpResource->setMaterial = true;
	}

	void SpriteEffectBuilder::BuildBinaryEffect(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->isSkinnedMesh = true;

			objRes->setMesh = true;

			objRes->setMaterial = true;

			objRes->ObjName = mesh;

			DXObject->SetMeshResources(objRes);
		}
	}
}

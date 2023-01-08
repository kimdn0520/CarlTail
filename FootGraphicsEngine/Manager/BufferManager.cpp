#include "GraphicsPch.h"
#include "BufferManager.h"
#include "Object/Buffer/ConstantBuffer.h"
#include "Util/LightHelper.h"

namespace GraphicsEngineSpace
{
	// �ν��Ͻ� �ʱ�ȭ
	std::shared_ptr<BufferManager> BufferManager::instance = nullptr;

	BufferManager::~BufferManager()
	{

	}

	std::shared_ptr<BufferManager> BufferManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<BufferManager>();

		return instance;
	}

	void BufferManager::Init(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext)
	{
		device = _device;
		deviceContext = _deviceContext;

		// ���� �ϳ� ����� ���ô�.
			// �������� typeSize�� �ȿ� ���� ��ü�� ����� ���´�..
		CreateConstantBuffer("MatrixCB", sizeof(SimpleMath::Matrix) * 3);
		CreateConstantBuffer("WorldViewProjCB", sizeof(SimpleMath::Matrix));
		CreateConstantBuffer("ColorCB", sizeof(SimpleMath::Vector4));
		CreateConstantBuffer("TexOffsetCB", sizeof(SimpleMath::Vector4));

		CreateConstantBuffer("ViewPosCB", sizeof(SimpleMath::Vector4));

		// ���� 4�� �� �� ��ŭ�� ũ��.
		CreateConstantBuffer("DLightCB", sizeof(SimpleMath::Vector4) * 4);
		CreateConstantBuffer("LegacyMaterialCB", sizeof(SimpleMath::Vector4) * 3);
		// PBRMaterialCB
		CreateConstantBuffer("PBRMaterialCB", sizeof(SimpleMath::Vector4) * 4);
		CreateConstantBuffer("PLightCB", sizeof(SimpleMath::Vector4) * 3);

		// Deferred Light
		CreateConstantBuffer("LightCB", sizeof(cbLight));

		// Bone Matrix
		CreateConstantBuffer("BoneMatrixCB", sizeof(SimpleMath::Matrix) * 100);
		// ���⼭ ������ ���۸� ������ �ͼ� ������ �� �������ش�.
	}


	bool BufferManager::CreateConstantBuffer(std::string cbName, UINT typeSize)
	{
		std::shared_ptr<BufferBase> tmpcbBuffer = std::make_shared<ConstantBuffer>(deviceContext);

		if( std::dynamic_pointer_cast<ConstantBuffer>(tmpcbBuffer) != nullptr)
		{
			if(std::dynamic_pointer_cast<ConstantBuffer>(tmpcbBuffer)->CreateBuffer(device, typeSize) != true)
				return false;
		}

		// ���۸� ����� ����. �ʿ� �־��ݴϴ�.
		bufferMap.insert(std::pair{cbName, tmpcbBuffer});

		return true;
	}

	void BufferManager::Finalize()
	{
		for(auto buffer : bufferMap)
		{
			buffer.second.reset();
		}

		bufferMap.clear();

		SafeReset(instance);
	}

	std::shared_ptr<BufferBase> BufferManager::GetBuffer(std::string bufferName)
	{
		if(bufferMap.find(bufferName) == bufferMap.end())
			return nullptr;

		std::shared_ptr<BufferBase> tmpBuf = bufferMap.at(bufferName);

		return bufferMap.at(bufferName);
	}
}

#pragma once
#include <map>

#include "Interface/IBuilder.h"
#include "GraphicsCore/RasterizerState.h"

/// Dll ��ũ��
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief �ܺο��� �������� ����ϱ� ���� ������ ������ �̱��� �Ŵ���
	 *
	 * ������(�׷��Ƚ� ����) ���ο��� ������ ����ϴ� ���� �ƴ�, ���� ���� Ȥ�� Ŭ���̾�Ʈ ��ġ���� ������ ����ؾ��ϱ� ������
	 * ���� �Լ����� ����� �� �ֵ��� �ܺη� �̾Ƴ��� Ŭ�����̴�.
	 * �ش� Ŭ�������� ����̽�, DC, RS ���� �ʱ�ȭ �ϰ�, ����� �� �̾��ش�.
	 *
	 * 2022.07.12(ȭ) Dev.BlackFoot
	 */
	class BuilderManger
	{
	private:
		// �ܺο��� �޾��� �ν��Ͻ�
		static std::shared_ptr<BuilderManger> instance;

		// ������ ������ ��
		std::map<std::string, std::shared_ptr<IBuilder>> builderMap;

		// ���ӿ��� or Ŭ���̾�Ʈ���� ������ �߰��Ѵٰ� ���� ��.. Device�� DC���� ������ �� ���� ������ �װ� ���⼭ ������ �ִ´�.
		// �װ� �ƴ϶�� ������ �߰� �� ������ �� Ŭ������ �������� ������ �߰����ִ� ����� �ִ�.(�ϵ� �ڵ�)
			// ����(22.07.12) Effect Ŭ������ InputLayout Ŭ������ �׷��� �Ǿ��ִ�.
		// �ϴ��� ���⼭ DC���� ������ �ְ�, �ʿ� ������ �߰��� �� �־��ִ� ������� �Ѵ�.
		ComPtr<ID3D11Device> D3DDevice = nullptr;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext = nullptr;
	public:
		// �׷��Ƚ� ���� �ȿ��� ����� �Լ�
		// ���� ����ϴ� ������ �ѹ��� Init
		void InitBuilderAll(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext);
		// ���ο� ������ ���� �߰��� ��..
			// T ��� ������ ��� ���� ������ �߰����ش�.
		template <typename T>
		void AddBuilder(std::string _builderName);

		// �׷��Ƚ� ���� �ܺο��� ��� ������ �Լ���.
		GRAPHICSENGINE_DECLSPEC static std::shared_ptr<BuilderManger> GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::make_shared<BuilderManger>();
			}

			return instance;
		}
		GRAPHICSENGINE_DECLSPEC void DeleteBuildManager();

		// Ư�� ������ �̾Ƴ���
		GRAPHICSENGINE_DECLSPEC std::shared_ptr<IBuilder> GetBuilder(std::string _builderName);
	};

	template <typename T>
	void BuilderManger::AddBuilder(std::string _builderName)
	{
		// ����ó��
		assert(D3DDevice);
		assert(D3DDeviceContext);
		assert(RasterizerState::GetSolidRS());
		assert(RasterizerState::GetWireFrameRS());

		// �������� �ñ״��ĸ� ��ġ��Ų��.
		std::shared_ptr<IBuilder> _tmpBuilder = std::make_shared<T>();

		// ���� �ʱ�ȭ
			// ��� ������ Init�� ���� �Ǿ��־���Ѵ�.
		_tmpBuilder->InitBuilder(D3DDevice, D3DDeviceContext);

		// �ش� ������ �߰���Ų��.
		builderMap.insert({ _builderName, _tmpBuilder });
	}
}

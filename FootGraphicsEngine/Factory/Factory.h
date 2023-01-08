#pragma once
#include "Interface/IBuilder.h"

/// Dll ��ũ��
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;
	class IBuilder;
	// ���丮�� ������Ʈ ������ �ʿ��� ����̽��� DC�� ������ ������ ������ �����Ѵ�.

	/**
	 * \brief ������ ���ؼ� ���� ������Ʈ�� ����� ���丮 �Լ�..
	 *
	 * ���������� �ʿ��� �Լ��� dllexport �Ѵ�.
	 * 2022.06.21(ȭ) Dev.BlackFoot
	 */
	class Factory
	{
	private:
		static std::shared_ptr<Factory> instance;

	public:
		// ���ø��� dll�� ���� �� ����. �ٸ� ����� �����ϱ� ������ �̰� �״�� ����� �� ���� ������.

		// Ư�� ��Ȳ���� �߰��Ǵ� args���� �޶����� ������.. �ش��ϴ� ������ �ѹ��� ó���� �� �ִ� ���� ���� ���ø��� ���.
			// ��ǻ� DiffuseMap�� NormalMap�ؽ��� ���� �Լ��� Builder���� ���鵵�� �ϱ� ���� �Լ��̴�.
		template <typename T>
		std::shared_ptr<IDXObject> CreateDXObject(std::shared_ptr<IBuilder> _builder, std::string _objName);

		GRAPHICSENGINE_DECLSPEC static std::shared_ptr<Factory> GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::make_shared<Factory>();
			}

			return instance;
		}
		GRAPHICSENGINE_DECLSPEC void DeleteFactory();
	};

	// Ư�� ��Ȳ�� �´� ������Ʈ�� ��� �޴� ģ���� ����� �ַ��� �ϱ� ���� ���� ���� ���ø��� ���.

	/**
	 * \brief IDXObject�� ��ӹ޴� ������Ʈ�� ����� ������ �߰��ϴ� �Լ�
	 * \tparam T ������� ������Ʈ�� ����
	 * \tparam Types �������ڷ� ���� Ÿ�Ե�
	 * \param _builder ������Ʈ�� ������ �־��� ����
	 * \param _objName ������Ʈ�� �̸�
	 * \param args ���� ���ڵ�, ���⼭�� ��ǻ��ʰ� �븻�� ��..
	 * \return 
	 */
	template <typename T>
	std::shared_ptr<IDXObject> Factory::CreateDXObject(std::shared_ptr<IBuilder> _builder, std::string _objName)
	{
		// ��ǻ� ���� ����.
		// �켱 ������Ʈ�� �����
		std::shared_ptr<IDXObject> _tmpObj = std::make_shared<T>();

		// ������ ���� ������ �߰����ִµ�, �ش� �Լ��� �������ڸ� ������.
		_tmpObj = _builder->BuildDXObject(_tmpObj, _objName);

		return _tmpObj;
	}
}

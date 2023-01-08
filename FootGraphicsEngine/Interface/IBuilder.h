#pragma once
#include "Resources/ObjectResources.h"
#include "Define/GraphicsRenderingData.h"

namespace GraphicsEngineSpace
{

	class IDXObject;

	/**
	* \brief ������ ������ ���������� ������ IBuilder Ŭ����.
	*/
	class IBuilder
	{
	public:
		virtual ~IBuilder() {}

		// ������ ������Ʈ�� �޾Ƽ�, ������Ʈ�� �������ִ� �Լ��� �����
		// ObjID�� �־� �ش� Obj�� �ִ��� Ȯ���Ѵ�.
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) abstract;

		// �������� ���ο� �����ϴ� ��� ������ �ʱ�ȭ ���ִ� Init �Լ�
			// ���� ���丮�� �ִ� Init�� �����´ٰ� ����.
		virtual void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) abstract;

	protected:
		// �ܺο� �巯�� �ʿ䰡 ����̴� �Լ�.
		// �����δ� ������Ʈ�� �ƴ϶� ���� ���ο��� ������Ʈ�� �������� �߰��ؼ� ������Ʈ�� �־��ش�.
			// ������Ʈ�� ���� ������ �������ִ� �����̳�(BuildInfo)�� ������ �ְ�
			// �ش� �����̳��� ������ �������� ä���� ��ȯ �� ��.
			// ������Ʈ�� �������ش�.
			// �ҿ� ������ ����.
		virtual std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) abstract;

	};

}
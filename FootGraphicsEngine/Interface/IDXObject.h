#pragma once
#include "Resources/ObjectResources.h"

/// Dll ��ũ��
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IRenderer;

	/**
	 * \brief Factory���� �����ϴ� ��� ������Ʈ�� ���̽��� �Ǵ� DXObj..
	 *
	 * �۵� ����� Interface�� �̾ƾ��Ѵ�.
	 * �⺻������ Device, DC. ���� ���ҽ��� ������ �־�� �Ѵ�.(�ش� ������ �� �������̽��� ��ӹ��� ������Ʈ�鿡 �־��ش�)
	 * 2022.06.21(ȭ) Dev.BlackFoot
	 */
	class GRAPHICSENGINE_DECLSPEC IDXObject
	{
	public:
		IDXObject() = default;
		virtual ~IDXObject() = default;

		// �ʱ�ȭ. Device�� DC�� �޾Ƽ�? �����غ��� �ش� ������.. ���ӿ��������� �� �ʿ䰡 ����������..
		// ����Ʈ �����ͷ� �ٲ��൵ ������ ��. 
		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) abstract;
		// ���� ����� �޾Ƽ� ������Ʈ
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) abstract;
		
		// tick�� �޾Ƽ� ������Ʈ ���ִ� PreRender
		virtual void PreRender(float tick) abstract;
		// ������ ������Ʈ�� ��� ������ �� ����̽��� �����͸� ������ �ִ� ���� ������..
		virtual void Render() abstract;

		// �ļ� �κ��� �������� ��..��.. �� ��� ����..
			// �������� ���� ������Ʈ�� Device�� �־��ش�.
		// ������Ʈ�� ���� �־��ֱ�
		virtual void SetMeshResources(std::shared_ptr<MeshResources> meshResources) abstract;
		virtual void SetBoneResource(std::shared_ptr<BoneResources> boneResources) {}
		virtual void SetAnimationResource(std::shared_ptr<AnimationResources> animResources) {}

		// ������Ʈ�� �з��� �� �ִ� �̸��� �־�����..
		virtual std::string GetObjName() abstract;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() abstract;
		virtual std::shared_ptr<BoneResources> GetBoneResources() { return nullptr; }
		virtual std::shared_ptr<AnimationResources> GetAnimationResources() { return nullptr; }

		// �ִϸ��̼� �����
		virtual void PlayAnim(std::string animClipName, bool isLoop) {};
	};
}

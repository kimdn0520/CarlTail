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
		//virtual std::shared_ptr<AnimationResources> GetAnimationResources() { return nullptr; }

		// ���� Matrix Getter
		virtual const SimpleMath::Matrix& GetWorld() abstract;
		virtual const SimpleMath::Matrix& GetView() abstract;
		virtual const SimpleMath::Matrix& GetProj() abstract;

		////////////////////////////////////////////////////////////////////////////////////////////////
		//										Animator ����
		////////////////////////////////////////////////////////////////////////////////////////////////
		 
		// �ִϸ��̼� �����
		virtual void PlayAnim(std::string animClipName, bool isLoop) {};

		// State �̸�, Ŭ�� �̸�, ��� �ӵ�, ���� ����
		virtual void AddAnimationState(const std::string& stateName, const std::string& animClipName, float speed, bool loop) {};

		// Transition�� �߰��� State�̸�, ���� State�̸�, �� State�̸�, hasExitTime, exitTime, Transition Duration
		virtual void SettingAnimationStateTransition(const std::string& stateName, const std::string& stateStart, const std::string& stateEnd, bool hasExitTime, float exitTime, float transitionDuration) {};
		
		// Entry�� ������ Aniamtion State
		virtual void EntryPlayAnimationState(const std::string& stateName) {};
		
		// Transition �̸�, Parameter �̸�, ���� �̸�, ��(���°� Ʈ����)
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, float val) {};
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, int val) {};
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, bool val) {};
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName) {};

		virtual void AddFloat(const std::string& name, float val) {};
		virtual void AddInteger(const std::string&, int val) {};
		virtual void AddBool(const std::string&, bool val) {};
		virtual void AddTrigger(const std::string& name) {};

		virtual void SetFloat(const std::string& name, float val) {};
		virtual void SetInteger(const std::string& name, int val) {};
		virtual void SetBool(const std::string& name, bool val) {};
		virtual void SetTrigger(const std::string& name) {};

		virtual int GetAnimCurrentFrame() { return 0; }
		virtual int GetAnimCurrentTotalFrame() { return 0; }
	};
}

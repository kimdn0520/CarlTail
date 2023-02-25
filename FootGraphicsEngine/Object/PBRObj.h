#pragma once
#include "Interface/IDXObject.h"
#include "Interface/IRenderer.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;
	class Animator;
	//class IRenderer;

	// ��� ��ü�� ������ ���� �ʾƵ� �Ǵ� bone struct�� ����� ���ش�
		// Skinned mesh�� ������ ������ �Ǳ� ����
	__declspec(align(16)) struct cbBoneMatrix
	{
		SimpleMath::Matrix boneMatrix[120];
	};

	/**
	 * \brief PBR���� ȭ�鿡 �׸��� PBR Obj
	 */
	class PBRObj : public IDXObject
	{
		// ��� ���� ���
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// ���� ������ ���Ե� ���ҽ�
			// Mesh�� ������ ������ �� �ִ�.
		std::vector<std::shared_ptr<MeshResources>> meshResources;
		std::shared_ptr<BoneResources> boneResources;
		//std::shared_ptr<AnimationResources> animResources;

		// ResourceManager ĳ�� �뵵
		std::shared_ptr<ResourceManager> resourceManager;

		std::unique_ptr<Animator> objAnimator;

		std::vector<SimpleMath::Matrix> nowBoneTM;

		// ������ ���� ���� ������ => Deferred�� ���� ������ Pass�� �ű��.

		// TM ���
		SimpleMath::Matrix world;
		SimpleMath::Matrix view;
		SimpleMath::Matrix proj;

		// ���̴��� ������ ��Ʈ����
		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};

		// PBR Material;
		__declspec(align(16)) struct cbMaterial
		{
			float metallic;
			float roughness;
			SimpleMath::Vector2 pad1;

			SimpleMath::Vector3 AddColor;
			float pad2;

			SimpleMath::Vector3 emissiveColor;
			float pad3;

			BOOL isLight = true;
		};


	public:
		GRAPHICSENGINE_DECLSPEC PBRObj();
		~PBRObj() override;

		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) override;
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;

		// deferred pass�� �ű� ���̱� ������ ���������� ������ ���� ����
		virtual void PreRender(float tick) override;
		virtual void Render() override;
		virtual void SetMeshResources(std::shared_ptr<MeshResources> objResources) override;
		virtual void SetBoneResource(std::shared_ptr<BoneResources> boneResources) override;
		virtual void SetAnimationResource(std::shared_ptr<AnimationResources> animResources) override;

		virtual std::string GetObjName() override;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;
		//virtual std::shared_ptr<AnimationResources> GetAnimationResources() override { return objAnimator->animResources; }

		virtual void PlayAnim(std::string animCilpName, bool isLoop) override;

		virtual void AddAnimationState(const std::string& stateName, const std::string& animClipName, float speed, bool loop) override;
		virtual void SettingAnimationStateTransition(const std::string& stateName, const std::string& stateStart, const std::string& stateEnd, bool hasExitTime, float exitTime, float transitionDuration) override;
		virtual void EntryPlayAnimationState(const std::string& stateName) override;
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, float val) override;
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, int val) override;
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, bool val) override;
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName) override;

		virtual void AddFloat(const std::string& name, float val) override;
		virtual void AddInteger(const std::string& name, int val) override;
		virtual void AddBool(const std::string& name, bool val) override;
		virtual void AddTrigger(const std::string& name) override;

		virtual void SetFloat(const std::string& name, float val) override;
		virtual void SetInteger(const std::string& name, int val) override;
		virtual void SetBool(const std::string& name, bool val) override;
		virtual void SetTrigger(const std::string& name) override;

		virtual int GetAnimCurrentFrame() override;
		virtual int GetAnimCurrentTotalFrame() override;

		virtual const SimpleMath::Matrix& GetWorld() override { return world; }
		virtual const SimpleMath::Matrix& GetView() override { return view; }
		virtual const SimpleMath::Matrix& GetProj() override { return proj; }

		const std::vector<SimpleMath::Matrix>& GetNowBoneTM() { return nowBoneTM; }

	private:
		void StaticRender(const std::shared_ptr<MeshResources>& objRes);
		void SkinnedRender(const std::shared_ptr<MeshResources>& objRes);
	};
}
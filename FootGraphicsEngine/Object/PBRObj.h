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
		SimpleMath::Matrix boneMatrix[100];
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
		std::shared_ptr<AnimationResources> animResources;

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
		virtual void SetBoneResource(std::shared_ptr<BoneResources> boneResources) override { this->boneResources = boneResources; }
		virtual void SetAnimationResource(std::shared_ptr<AnimationResources> animResources) override { this->animResources = animResources; }

		virtual std::string GetObjName() override;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;
		virtual std::shared_ptr<AnimationResources> GetAnimationResources() override { return animResources; }

		virtual void PlayAnim(std::string animCilpName, bool isLoop) override;

	private:
		void StaticRender(const std::shared_ptr<MeshResources>& objRes);
		void SkinnedRender(const std::shared_ptr<MeshResources>& objRes);
	};
}
#pragma once
#include "Define/d3d11Utils.h"

namespace GraphicsEngineSpace
{
	class Mesh;

	class Texture;

	class RenderingMaterial;

	// �� �ϳ��� ������
	struct Bone
	{
		SimpleMath::Matrix offsetTransform;
		SimpleMath::Matrix nodeTransform;
		int parentIdx;

		Bone()
			: offsetTransform(SimpleMath::Matrix::Identity)
			, nodeTransform(SimpleMath::Matrix::Identity)
			, parentIdx(-1)
		{
			
		}

		~Bone() = default;
	};

	// ��Ű�� ���� �� ������.
	struct BoneResources
	{
		bool isSkinnedObj;
		std::vector<Bone> bones;

		// ��� TM�� �̹� ������°�..
		bool setNode;

		BoneResources()
			: isSkinnedObj(false)
			, setNode(false)
		{
			
		}

		~BoneResources() = default;
	};

	// �ִϸ��̼ǿ� ���� ���� �������� KeyFrame ����
		// ������ ���� ���� �Ǿ�� �ϴ� �ִϸ��̼� ����.
	struct KeyFrame
	{
		// Local ������.
		SimpleMath::Vector3 framePos;
		SimpleMath::Quaternion frameRot;
		SimpleMath::Vector3 frameScale;

		float time;

		KeyFrame()
			: framePos(0.f, 0.f, 0.f)
			, frameRot(0.f, 0.f, 0.f, 0.f)
			, frameScale(0.f, 0.f, 0.f)
			, time(0.f)
		{

		}

		~KeyFrame() = default;
	};

	// �ִϸ��̼� Ŭ�� �ϳ��ϳ��� �����ϴ� Struct
	struct AnimationClipResource
	{
		std::string animationName;

		// �ش� Ŭ���� ��� KeyFrame�� �����ص� Vector
		// BoneResources�� ���� �� ������ �ش� ���ҽ��� ���� Ű ������ ������ ��ġ�Ѵ�.
		std::vector<std::vector<KeyFrame>> keyFrames;

		// ������ Rate
		float frameRate;

		// �ִϸ��̼� ù ��° ������
		int startKeyFrame;

		// �ִϸ��̼� �� ������ ��
		int totalKeyFrame;

		// �ִϸ��̼� ������ ������
		int endKeyFrame;

		// �����Ӵ� �� Tick�� ����������
		float tickPerFrame;

		AnimationClipResource()
			: animationName("NONE")
			, frameRate(0.f)
			, startKeyFrame(0)
			, totalKeyFrame(0)
			, endKeyFrame(0)
			, tickPerFrame(0.f)
		{
			
		}

		~AnimationClipResource() = default;
	};

	// ��Ű�� ���� �ִϸ��̼� ������
		// �ش� Object�� ������ �ִ� ��� �ִϸ��̼� ������ ��Ƶ� Resources
		// Animation Clip���� List�� ������ �ִ�.
		// (�� �ִϸ��̼� Clip�� �̸� ���� ������ ���� Animator���� ���� �����ϵ��� �����.)
	struct AnimationResources
	{
		std::unordered_map<std::string, std::shared_ptr<AnimationClipResource>> animationClipList;

		AnimationResources()
		{
			
		}

		~AnimationResources() = default;
	};

	/**
	* \brief ������Ʈ�� ������ �ִ� ���� ���ҽ���
	*
	* // �ش� ������ ������ ������ ����, ������Ʈ�� ������ ����. �������� �����ؼ� ������Ʈ�� �־��ش�.
	*
	* 2022.07.11(��) Dev.BlackFoot
	*/
	struct MeshResources
	{
		bool setMesh;

		bool setMaterial;

		std::shared_ptr<Mesh> mesh;

		std::vector<std::shared_ptr<RenderingMaterial>> material;	

		// ������Ʈ �̸�
		std::string ObjName;

		// �޽� ��ü�� world TM.. => ���⼭ ó�����ش�.
		SimpleMath::Matrix nodeTM;

		// Skinned ���� ���� ���⼭ ������ �ִ´�.
		bool isSkinnedMesh;

		MeshResources()
			: setMesh(false)
			, setMaterial(false)
			, ObjName("NULL")
			, nodeTM(SimpleMath::Matrix::Identity)
			, isSkinnedMesh(false)
		{}

		~MeshResources()
		{
			
		}
	};
}
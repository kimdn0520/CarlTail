#pragma once

namespace GraphicsEngineSpace
{
	class KeyFrame;
	class AnimationClipResource;

	// ���� �ִϸ��̼� Ŭ���� ������ �ְ�,
	// Tick�� �޾� ���� �����ӿ� ������ ���� �������� Bone TM�� ������ִ� Helper Class
	// 2022.12.22 Dev.BlackFoot
	class Animator
	{
	private:
		// ���� AnimClip
		std::shared_ptr<AnimationClipResource> nowAnimClip;

		// ���� Ű �������� index������ ������ ���� ����
		int nowKeyFrameIdx;

		// ���� �ݺ� �ִϸ��̼����� �� ������ ����.
		bool isLoop;

		// ��� ����
		bool isPlay;

		// DeltaTick���� ���� ���� ������ Ÿ��
		float nowFrameTime;

		// ������ ������ BoneTransForm
		std::vector<SimpleMath::Matrix> modifiedBoneTM;

	public:
		// BoneSize�� �޾Ƽ� Vector�� �ʱ�ȭ���� �ش�.
		Animator(int boneSize);
		virtual ~Animator();

		// DeltaTick�� �޾� �ð��� ����ϸ鼭 ���ο� BoneTM�� ���� �̾��ش�.
			// KeyFrame���� Bone Idx���� ����ؼ� �����ѵ� �־��ش�..
		std::vector<SimpleMath::Matrix> UpdateBoneTM(float tick);

		// Object���� ������ animationClip�� ������ �� �ְ� ���ִ� �Լ�
			// loop ���α��� ����
		virtual void SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop);
		
	};

}
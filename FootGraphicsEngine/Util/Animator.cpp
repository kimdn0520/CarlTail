#include "GraphicsPch.h"
#include "Animator.h"

#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	Animator::Animator(int boneSize)
		: nowAnimClip(nullptr)
		, nowKeyFrameIdx(0)
		, isLoop(true)
		, isPlay(true)
		, nowFrameTime(0.f)
	{
		// �ش� obj�� ������ �ִ� �� ũ�� ��ŭ�� Matrix vector�� �������ش�.
		modifiedBoneTM.assign(boneSize, SimpleMath::Matrix::Identity);
	}

	Animator::~Animator()
	{
	}

	std::vector<SimpleMath::Matrix> Animator::UpdateBoneTM(float tick)
	{
		// ���������� ���͸� ������ �־�� �ش� ������ ���� ������� �ʰ� ���� �ȴ�.
		// ���, ���� �� Matrix ������ �߰����ش�.

		// �ϴ� ���� ������ Animation�� �����غ���.
			// Frame Rate�� TickPerFrame ���� ���� �������� �ʴ´�.

		// �⺻������ �ִϸ��̼���
			// ���� FrameTime�� delta Tick�� ��� �����ָ鼭
			// FrameTime ���� ���� prev KeyFrame��
			// FrameTime ���� ū next KeyFrame�� �����ؼ� ���� TM�� ������.

		// ��� ���� �ľ�
		if (isPlay != true || nowAnimClip == nullptr)
			return modifiedBoneTM;

		KeyFrame nowKeyFrame;
		KeyFrame nextKeyFrame;

		// ������ Ű������ üũ, ���� üũ
		if (nowKeyFrameIdx == nowAnimClip->endKeyFrame)
		{
			if (isLoop == true)
				nowKeyFrameIdx = nowAnimClip->startKeyFrame;
			else
			{
				isPlay = false;	
				return modifiedBoneTM;	// ������ �����ӿ��� ����.
			}
		}

		// �ִϸ��̼� Ŭ���� ��� Ű�������� ���鼭..
			// cf)AnimationClip�� ����
			// ���ο� KeyFrames�� �ٽ�
			// n������ m��° Frame�� �������.
		// �� ��� ���� ������ �߰��ؾ� �ϱ� ������.. ��� ���� ���������
		for (size_t i = 0; i < modifiedBoneTM.size(); i++)
		{
			// �ִϸ��̼� Ŭ���� �ش� Ű�����ӿ� ����
			nowKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx];
			// ������ ������ üũ
			if(nowKeyFrameIdx + 1 == nowAnimClip->endKeyFrame)
			{
				if(isLoop == true)
					nextKeyFrame = nowAnimClip->keyFrames[i][nowAnimClip->startKeyFrame];
				else
					nextKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx];
			}

			// TODO ���� �� Matrix�� ���� �־��ش�.
			// (SRT)
			SimpleMath::Matrix nowTM = SimpleMath::Matrix::CreateScale(nowKeyFrame.frameScale)
			* SimpleMath::Matrix::CreateFromQuaternion(nowKeyFrame.frameRot)
			* SimpleMath::Matrix::CreateTranslation(nowKeyFrame.framePos);

			// i��° ���� �ش� TM�� �־��ش�.
			modifiedBoneTM[i] = nowTM;
		}

		// KeyFrame TimeCheck (������ next KeyFrame�� ���Դٰ� ����.)
			// ���� keyFrame�� Time���� nowFrameTime�� ũ�ٸ�.
			// ���� Ű���������� �Ѱ��ش�.
		if(nextKeyFrame.time < nowFrameTime)
			nowKeyFrameIdx++;

		nowFrameTime += tick;

		// ���� ������ Ÿ���� ���� ������ Ÿ���� �Ѿ��. ���ʷ� �ǵ����ش�.
		int endKeyFrameIdx = nowAnimClip->endKeyFrame - 1;
		if(nowFrameTime > nowAnimClip->keyFrames[0][endKeyFrameIdx].time)
		{
			nowFrameTime = nowAnimClip->keyFrames[0][nowAnimClip->startKeyFrame].time;
		}

		// ���������� �ش� TMVector�� �������ش�.
		return modifiedBoneTM;
	}

	void Animator::SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop)
	{
		nowAnimClip = animClip;
		// ù Ű ������ ���.
		nowKeyFrameIdx = nowAnimClip->startKeyFrame;

		this->isLoop = isLoop;
	}
}

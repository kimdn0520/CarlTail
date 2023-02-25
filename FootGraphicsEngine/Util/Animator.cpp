#include "GraphicsPch.h"
#include "Animator.h"

#include "Resources/ObjectResources.h"

#include "Util/AnimatorState.h"
#include "Util/AnimatorParmeter.h"
#include "Util/AnimatorTransition.h"

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

		blendBoneTM.assign(boneSize, SimpleMath::Matrix::Identity);

		// Parameter ����
		animatorParameter = std::make_shared<AnimatorParmeter>();

		// Any State ����
		//std::shared_ptr<AnimatorState> anyState
	}

	Animator::~Animator()
	{
		if (animResources != nullptr)
		{
			for (auto animClip : animResources->animationClipList)
			{
				SafeReset(animClip.second);
			}
		}

		SafeReset(animResources);
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

		// ����� 02-20 ����
		// Aniamtor ���� & Aniamtion blending

		// state�� ��ȯ�� �Ǿ��ٸ�
		if (currentStateName.compare(currentAnimatorState->GetStateName()) != 0)
		{
			// transitionDuration �� ���� blending �� �Ѵ�.
			if (currentTransitionTime > 0.0f)
			{
				// ���� ��ȯ �ð��� ����
				currentTransitionTime -= tick;

				// blending �� �غ���..
				const float blendFactor = 1.0f - (currentTransitionTime / currentTransitionDuration);

				// ������ bone ���
				auto curBoneTM = modifiedBoneTM;
				
				KeyFrame nextStateStartFrame;

				nowAnimClip = animResources->animationClipList[states[currentStateName]->GetMotion()];

				for (size_t i = 0; i < blendBoneTM.size(); i++)
				{
					// �ִϸ��̼� Ŭ���� �ش� Ű�����ӿ� ����
					nextStateStartFrame = nowAnimClip->keyFrames[i][0];

					// (SRT)
					SimpleMath::Vector3 pos = nextStateStartFrame.framePos;
					SimpleMath::Quaternion rot = nextStateStartFrame.frameRot;
					SimpleMath::Vector3 scale = nextStateStartFrame.frameScale;

					SimpleMath::Matrix nextStateStartFrameTM = SimpleMath::Matrix::CreateScale(scale)
														* SimpleMath::Matrix::CreateFromQuaternion(rot)
														* SimpleMath::Matrix::CreateTranslation(pos);

					blendBoneTM[i] = curBoneTM[i] * (1.0f - blendFactor) + nextStateStartFrameTM * blendFactor;
				}

				return blendBoneTM;
			}
			else
			{
				// ���� ���¸� �������ش�.
				currentAnimatorState = states[currentStateName];

				nowKeyFrameIdx = 0;

				currentTransitionTime = 0.0f;

				// ���� �� ���� �ִϸ��̼� �ٷ� return ����
				nowFrameTime = 100.0f;
				
				return UpdateFrame(tick);
			}
		}
		else
		{	
			currentStateName = currentAnimatorState->UpdateState(animatorParameter);

			if (currentAnimatorState->operateTransition != nullptr)
			{
				currentTransitionTime = currentAnimatorState->operateTransition->GetTransitionDuration();

				currentTransitionDuration = currentTransitionTime;

				currentAnimatorState->operateTransition = nullptr;
			}

			return UpdateFrame(tick);
		}
	}

	std::vector<SimpleMath::Matrix> Animator::UpdateFrame(float tick)
	{
		nowAnimClip = animResources->animationClipList[currentAnimatorState->GetMotion()];

		isLoop = currentAnimatorState->GetLoop();

		// ��� ���� �ľ�
		if (isPlay != true || nowAnimClip == nullptr)
			return modifiedBoneTM;

		nowFrameTime += tick;

		// ���� �� ����� ���� idle->run �Ҷ� ��� idle ��¦�ϴ� ����..
		if (nowFrameTime < 1.0f / nowAnimClip->frameRate)
			return modifiedBoneTM;

		nowFrameTime = 0.f;

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

		// ������ �ʿ��ϴ�.
		float currentTick = nowKeyFrameIdx * nowAnimClip->tickPerFrame;
		float t = 0.f;

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
			if (nowKeyFrameIdx + 1 == nowAnimClip->endKeyFrame)
			{
				if (isLoop == true)
					nextKeyFrame = nowAnimClip->keyFrames[i][nowAnimClip->startKeyFrame];
				else
					nextKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx];
			}
			else
				nextKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx + 1];

			// TODO ���� �� Matrix�� ���� �־��ش�.
			t = (currentTick - nowKeyFrame.time) / (nextKeyFrame.time - nowKeyFrame.time);

			// (SRT)
			SimpleMath::Vector3 pos = XMVectorLerp(nowKeyFrame.framePos, nextKeyFrame.framePos, t);
			SimpleMath::Quaternion rot = XMQuaternionSlerp(nowKeyFrame.frameRot, nextKeyFrame.frameRot, t);
			SimpleMath::Vector3 scale = XMVectorLerp(nowKeyFrame.frameScale, nextKeyFrame.frameScale, t);

			SimpleMath::Matrix nowTM = SimpleMath::Matrix::CreateScale(scale)
				* SimpleMath::Matrix::CreateFromQuaternion(rot)
				* SimpleMath::Matrix::CreateTranslation(pos);
			/*
			SimpleMath::Matrix nowTM = SimpleMath::Matrix::CreateScale(nowKeyFrame.frameScale)
			* SimpleMath::Matrix::CreateFromQuaternion(nowKeyFrame.frameRot)
			* SimpleMath::Matrix::CreateTranslation(nowKeyFrame.framePos);
			 */

			 // i��° ���� �ش� TM�� �־��ش�.
			modifiedBoneTM[i] = nowTM;
		}

		// KeyFrame TimeCheck (������ next KeyFrame�� ���Դٰ� ����.)
			// ���� keyFrame�� Time���� nowFrameTime�� ũ�ٸ�.
			// ���� Ű���������� �Ѱ��ش�.
		nowKeyFrameIdx++;

		return modifiedBoneTM;
	}

	void Animator::SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop)
	{
		nowAnimClip = animClip;
		// ù Ű ������ ���.
		nowKeyFrameIdx = nowAnimClip->startKeyFrame;

		this->isLoop = isLoop;
	}

	int Animator::GetTotalFrame()
	{
		return nowAnimClip->totalKeyFrame;
	}


	void Animator::SetAnimatorState(const std::string& stateName, std::shared_ptr<AnimatorState> animatorState)
	{
		states.insert(std::make_pair(stateName, animatorState));
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, float val)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.floatVal = val;
		
		if(conditionName == "Greater")
			condition.type = ConditionType::Greater;
		else if(conditionName == "Less")
			condition.type = ConditionType::Less;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::EntryAnimatorState(const std::string& name)
	{
		currentStateName = name;

		currentAnimatorState = states[name];
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, int val)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.intVal = val;

		if (conditionName == "Greater")
			condition.type = ConditionType::Greater;
		else if (conditionName == "Less")
			condition.type = ConditionType::Less;
		else if(conditionName == "Equlals")
			condition.type = ConditionType::Equals;
		else if(conditionName == "NotEqual")
			condition.type = ConditionType::NotEqual;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, bool val)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.intVal = val;
		condition.type = ConditionType::None;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.type = ConditionType::None;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::AddFloat(const std::string& name, float val)
	{
		animatorParameter->AddFloat(name, val);
	}

	void Animator::AddInteger(const std::string& name, int val)
	{
		animatorParameter->AddInteger(name, val);
	}

	void Animator::AddBool(const std::string& name, bool val)
	{
		animatorParameter->AddBool(name, val);
	}

	void Animator::AddTrigger(const std::string& name)
	{
		animatorParameter->AddTrigger(name);
	}

	void Animator::SetFloat(const std::string& name, float val)
	{
		animatorParameter->SetFloat(name, val);
	}

	void Animator::SetInteger(const std::string& name, int val)
	{
		animatorParameter->SetInteger(name, val);
	}

	void Animator::SetBool(const std::string& name, bool val)
	{
		animatorParameter->SetBool(name, val);
	}

	void Animator::SetTrigger(const std::string& name)
	{
		animatorParameter->SetTrigger(name);
	}
}

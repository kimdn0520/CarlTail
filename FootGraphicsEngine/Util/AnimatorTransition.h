#pragma once

namespace GraphicsEngineSpace
{
	class AnimatorParmeter;

	enum class ConditionType
	{
		Greater,
		Less,
		Equals,
		NotEqual,

		None,
	};

	struct Condition
	{
		std::string parameterName;  // �Ķ���� �̸�

		ConditionType type;		// ���� Ÿ��

		float floatVal;			// ���� float ��

		int intVal;				// ���� int ��

		bool boolVal;			// ���� bool ��
	};

	class AnimatorTransition
	{
	private:
		// Transition State Name
		std::string transitionState;

		// State Start
		std::string	animationStateStart;

		// State End
		std::string	animationStateEnd;

		// ���� ������ Ȱ��ȭ �ϴ� �ɼ�
		// Ȱ��ȭ �� -> ���� ������ �����ϰ� �Ǿ� ������ ������ �����ص� ���� ���·� �������� �ʰ� �ȴ�.
		// ��Ȱ��ȭ �� -> ���� ������ �����ϴ� ��� ���̰� ���۵ȴ�.
		bool hasExitTime;

		// hasExitTime�� Ȱ��ȭ�ÿ� ������ �� �ִ�. (���� ����)
		float exitTime;

		// Ʈ�������� ���ӽð�, ���� �Ҷ� ���δ�.
		// ���� �ִϸ��̼� Ŭ���� ���� �ִϸ��̼� Ŭ���� ����(����) �ε巴�� �̾��ִ� ������ �Ѵ�.
		float transitionDuration;

		// ���� ����Ʈ
		std::vector<Condition> conditions;

	public:
		AnimatorTransition();
		~AnimatorTransition();

	public:
		const std::string& GetTransitionName() { return transitionState; }

		const std::string& GetAnimationStateStartName() { return animationStateStart; }

		const std::string& GetAnimationStateEndName() { return animationStateEnd; }

		const float GetTransitionDuration() { return transitionDuration; }

		void SetTransitionState(std::string stateStart, std::string stateEnd);

		void SetAnimationStateStart(std::string _animationStateStart) { animationStateStart = _animationStateStart; }
	
		void SetAnimationStateEnd(std::string _animationStateEnd) { animationStateEnd = _animationStateEnd; }

		void SetHasExitTime(bool val) { hasExitTime = val; }

		void SetExitTime(float val) { exitTime = val; }

		void SetTransitionDuration(float val) { transitionDuration = val; }

		void AddCondition(Condition& condition) { conditions.push_back(condition); }

		bool CheckCondition(std::shared_ptr<AnimatorParmeter>& parameter);
	};
}


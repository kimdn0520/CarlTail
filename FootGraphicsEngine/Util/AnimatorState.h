#pragma once

namespace GraphicsEngineSpace
{
	class AnimatorTransition;
	class AnimatorParmeter;

	class AnimatorState
	{
	private:
		std::string stateName;

		// �ִϸ��̼� Ŭ�� �̸�
		std::string motion;		

		// �ִϸ��̼� ��� �ӵ�
		float speed;

		// Loop ����
		bool isLoop;
		
		// �ش� State�� ����� transition ��
		std::vector<std::shared_ptr<AnimatorTransition>> transitions;

	public:
		// CheckCondition�� True�� �� transition
		std::shared_ptr<AnimatorTransition> operateTransition;

	public:
		AnimatorState();
		~AnimatorState();

	public:
		const std::string& GetStateName() { return stateName; }

		bool GetLoop() { return isLoop; }

		const std::string& GetMotion() { return motion; }

		void SetStateName(std::string _stateName) { stateName = _stateName; }

		void SetMotion(std::string _motion) { motion = _motion; }

		void SetSpeed(float _speed) { speed = _speed; }

		void SetLoop(bool val) { isLoop = val; }

		std::shared_ptr<AnimatorTransition> GetTransition(const std::string& name);

		void AddTransition(std::shared_ptr<AnimatorTransition> transition) { transitions.push_back(transition); }

		std::string UpdateState(std::shared_ptr<AnimatorParmeter> parameter);
	};
}

#pragma once

namespace GameEngineSpace 
{
	// Node Interface
	class Node 
	{  
	public:
		virtual bool run() = 0;
	};

	// children Node ����
	class CompositeNode : public Node 
	{ 
	private:
		std::list<std::shared_ptr<Node>> children;

	public:
		const std::list<std::shared_ptr<Node>>& getChildren() const { return children; }

		void addChild(std::shared_ptr<Node> child) { children.emplace_back(child); }
	};

	// Action(������ true�� ��ȯ)
	class ActionNode : public Node
	{
	private:
		std::function<bool()> action;
	
	public:
		ActionNode(const std::function<bool()>& _action)
			: action(_action)
			{}

	public:
		bool run() override { return action(); }
	};

	class ConditionNode : public Node 
	{
	private:
		std::function<bool()> condition;

	public:
		ConditionNode(const std::function<bool()>& _condition)
			: condition(_condition)
			{}
	
	public:
		bool run() override;
	};

	// ���� �ڽ� ��带 ������ ���  (looping, negation, and counting... �� �� ���鶧 ����Ѵ�.)
	class DecoratorNode : public Node 
	{
	private:
		std::shared_ptr<Node> child;

	public:
		DecoratorNode(const std::shared_ptr<Node>& _child)
			: child(_child) 
			{}

	public:
		bool run() override 
		{
			// child ��尡 run �� �Ǳ����� �����ؾ��� �۾�
			before();

			// child run
			bool result = child->run();

			// child ��尡 run �� �� �����ؾ��� �۾�
			after();
			return result;
		}

		virtual void before() = 0;
		virtual void after() = 0;
	};

	// �ϳ��� child�� �����ϸ� ��ü run() �۾��� �����Ѵ�. ��� child�� �����ϸ� fail
	class SelectorNode : public CompositeNode 
	{
	public:
		virtual bool run() override;
	};

	// �ϳ��� child�� �����ϸ� ��ü run() �۾��� �����Ѵ�. ��� child�� �����ؾ� ����� ����
	class SequenceNode : public CompositeNode 
	{
	public:
		virtual bool run() override;
	};
}


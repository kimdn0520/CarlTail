#pragma once
#include "ColliderBase.h"

namespace GameEngineSpace
{
	/**
	 * \brief �׺���̼� �޽ÿ� �۵��ϴ� ������Ʈ
	 *	..
	 *
	 * 2023.01.08(��) Dev.Newbie
	 */
	class NavMeshAgent : public ComponentBase
	{
	private:
		// Agent Size
		float radius;
		float height;
		float baseOffset;		// ��ȯ �ǹ����� �������� �浹 ������ �������Դϴ�.

		// Steering
		float speed;			// �ִ� �̵� �ӵ�
		float angularSpeed;		// �ִ� ���ӵ�
		float acceleration;		// �ִ� ���ӵ�
		float stopDistance;		// ��ǥ�� ����������� ���� �Ÿ�
		bool autoBreaking;		// Ȱ��ȭ�Ǹ� ������Ʈ�� ��� ������ �� �ӵ��� ������

	public:
		NavMeshAgent(std::weak_ptr<GameObject> _gameObject);
		virtual ~NavMeshAgent();

		void Release() override;
	};
}

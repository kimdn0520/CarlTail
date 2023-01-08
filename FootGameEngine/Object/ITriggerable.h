#pragma once

namespace GameEngineSpace
{
	class ColliderBase;

	/**
	 * \brief Ʈ���� �̺�Ʈ�� �ߵ���ų �� �ִ� ������Ʈ�鿡 �ٴ� Ʈ���� �ݶ��̴�.
	 *
	 * �ش� Ŭ������ ��ӹ��� �� Ʈ���� �̺�Ʈ�� �߻��ϸ� �������� �̿��ؼ� �ٸ��� �����ϰ� ���� �� �ִ�.
	 *
	 * 2022.08.03(��) Dev.BlackFoot
	 */
	class ITriggerable
	{
		// ���������� ������ �ȵǾ ������ ������ abstract�� ������ �ʴ´�.
		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) {}
		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) {}
		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) {}

		friend class GameObject;
	};

}

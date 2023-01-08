#pragma once
#include "Object/ComponentBase.h"

namespace GameEngineSpace
{
	class Transform;

	class PhysicsManager;

	/**
	 * \brief ��� �ݶ��̴����� �⺻�� �Ǵ� �ݶ��̴� ���̽� Ŭ����
	 *
	 * �⺻������ �������Ŵ��� ���� �����Ѵ�.
	 *
	 * 2022.08.03(��) Dev.BlackFoot
	 */
	class ColliderBase : public ComponentBase, public std::enable_shared_from_this<ColliderBase>
	{
	protected:
		// ĳ�̿� Ʈ������
		std::shared_ptr<Transform> transform;
		// ���� �浹�� �ݶ��̴���
		std::vector<std::shared_ptr<ColliderBase>> currColliedColList;
		// ������ �浹ó���� �� �ݶ��̴���
		std::vector<std::shared_ptr<ColliderBase>> prevColliedColList;

		bool isTrigger;

		Vector3 color;

	public:
		ColliderBase(std::weak_ptr<GameObject> gameObj);
		virtual ~ColliderBase();

		void Awake() override;

		// ������ �������� �̿��ؼ� �ݸ����� üũ���ִ� �Լ�
			// �� ������Ʈ�� ��ӹ޴� �ݶ��̴��� ��� �ݸ��� üũ�� ����� �־���Ѵ�.
			// ���Ŀ� �پ��� ��Ȳ�� ���缭 �� �ݶ��̴� ������Ʈ�� �߰����ָ� �ȴ�.
		virtual bool CheckCollision(std::shared_ptr<ColliderBase> other) abstract;

		// �������� �̿��ؼ� �ݶ��̴��� �����ϴ� �Լ�..
		virtual void ColliderRender() {}

		// ������ ������ �ʿ� ���� �Լ�
		void SetColor(const Vector3& _color) { color = _color; };

		void ClearCurrColliedColList() { currColliedColList.clear(); }
		bool GetIsCollidedThisFrame() { return currColliedColList.size() != 0; }

		friend class PhysicsManager;
	};

}

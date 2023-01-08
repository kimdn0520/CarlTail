#pragma once
#include "ColliderBase.h"

namespace GameEngineSpace
{
	class BoxCollider;

	/**
	 * \brief �� �浹�� �����ϴ� �ݶ��̴� ������Ʈ
	 * ���� �ٿ�� ������ ������� �ʰ�.. �߽ɰ� �Ÿ��� �Ǻ��Ѵ�.
	 *
	 * 2022.08.03 DevBlackFoot.
	 */
	class SphereCollider : public ColliderBase
	{
	private:
		// �ش� ������Ʈ�� ���� �󿡼��� �߾Ӱ�
		Vector3 localCenter;
		// ���忡���� �߾Ӱ�
		Vector3 worldCenter;
		// ������
		float radius;

	public:
		SphereCollider(std::weak_ptr<GameObject> gameObj);
		virtual ~SphereCollider();

	private:
		// �� ����� �浹 üũ
		bool CheckForSphere(std::shared_ptr<SphereCollider> other);
		bool CheckForBox(std::shared_ptr<BoxCollider> other);

	public:
		void Release() override;

		// Getter & Setter
		const Vector3& GetLocalCenter() { return localCenter; }
		const Vector3& GetWorldCenter() { return worldCenter; }
		const float& GetRadius() { return radius; }

		void SetLocalCenter(const Vector3& newCenter) { localCenter = newCenter; }
		void SetRadius(const float& _radius) { radius = _radius; }

		// �浹 üũ
		bool CheckCollision(std::shared_ptr<ColliderBase> other) override;

		// �ݶ��̴� ����
		void ColliderRender() override;

		void LateUpdate(float tick) override;

		friend class PhysicsManager;

		friend class BoxCollider;
		friend class RayCollider;
	};

}

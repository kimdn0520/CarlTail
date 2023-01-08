#pragma once
#include "ColliderBase.h"

namespace GraphicsEngineSpace
{
	class IDXObject;
}

namespace GameEngineSpace
{

	class SphereCollider;

	/**
	 * \brief �и��� �̷��� �̿�, OBB�� �ڽ� �浹�� �˻��ϴ� �ݶ��̴� Ŭ����
	 *
	 * ref by CometEngine
	 *
	 * 2022.08.04(��) Dev.BlackFoot
	 */
	class BoxCollider : public ColliderBase
	{
	private:
		// �⺻������ ���� �������� ���� ��ġ�� �˱� ���ؼ� �����̼� ����� �ʿ��ϴ�.
			// ȸ���� ������ü�� �浹�� �˻��ϱ� ����.
		Matrix objWorldRotTM;
		// ���ð� ������ ����
		Vector3 localCenter;
		Vector3 worldCenter;

		// �и������ ���� => �˻縦 ������ ����� �븻 ����.
		std::vector<Vector3> worldNormals;

		// �ڽ� �ϳ��� ������ �ִ� ������ == radius
		Vector3 size;
		Vector3 tmpSize;

		// �� ���� ������ �ִ� ������ ũ�� => �� ���Ϳ��� �󸶸�ŭ �Ÿ��� �������ִ°�..
		std::vector<float> extents;

		std::shared_ptr<GraphicsEngineSpace::IDXObject> colliderRenderObj;

	public:
		BoxCollider(std::weak_ptr<GameObject> gameObj);
		virtual ~BoxCollider();

		void Release() override;

	private:
		// �켱 �ڽ����� �浹��...
		bool CheckForBox(std::shared_ptr<BoxCollider> other);

		// ���Ŀ� �ڵ� �����ؼ� �� �浹��..
		bool CheckForSphere(std::shared_ptr<SphereCollider> other);

	public:
		// Getter Setter
		const Vector3& GetLocalCenter() { return localCenter; }
		const Vector3& GetWorldCenter() { return worldCenter; }
		const Vector3& GetSize() { return size; }

		void SetLocalCenter(const Vector3& newCenter) { localCenter = newCenter; }
		void SetSize(const Vector3& newSize);

		// �浹 üũ
		bool CheckCollision(std::shared_ptr<ColliderBase> other) override;

		void InitRenderObj(std::string _objName);
		// �ݶ��̴� ����
		void ColliderRender() override;

		void LateUpdate(float tick) override;

		friend class PhysicsManager;

		friend class SphereCollider;
		friend class RayCollider;
	};

}

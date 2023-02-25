//#pragma once
//
//#include "Collider.h"
//#include "Object/GameObject.h"
//
//namespace GameEngineSpace
//{
//	class BoxCollider;
//	class SphereCollider;
//
//	/**
//		 * \brief ����ĳ���� ���� �浹�� ��ü�� ������ ������ �ִ� ���� ĳ��Ʈ Hit Ŭ����..
//		 *
//		 * �⺻�����δ� ���� ��ü�� ���ӿ�����Ʈ, Ʈ������, name, �Ÿ� ������ ������ �ִ�.
//		 */
//	struct RaycastHit
//	{
//		std::shared_ptr<GameObject> hitGameObject;
//		std::shared_ptr<Transform> hitTransform;
//		float hitDist;
//		tstring hitName;
//
//		RaycastHit()
//			: hitGameObject(nullptr)
//			, hitTransform(nullptr)
//			, hitDist(0.f)
//			, hitName(TEXT("HitNone"))
//		{
//		}
//	};
//
//	/**
//	 * \brief ���� ĳ������ ���� �����ݶ��̴� ������Ʈ.
//	 *
//	 * �ش� ������Ʈ�� �پ��ִ� ���ӿ�����Ʈ�� Look ���Ϳ� Center�� �̿��ؼ� Origin -> Dir�� ���ϴ� ���̸� ���.
//	 *
//	 * 2022.08.08(��) Dev.BlackFoot
//	 */
//	class RayCollider : public Collider
//	{
//	private:
//		// ��ġ ���Ϳ� ���� ����.
//		Vector3 localCenter;
//		Vector3 worldCenter;
//		Vector3 direction;
//
//		// ���� ó�� �浹�� �ݶ��̴� ���� ������ ����
//		RaycastHit hit;
//
//		// �Ÿ� �� ������ ����
//		std::vector<std::pair<std::shared_ptr<GameObject>, float>> currHitObjVec;
//
//	public:
//		RayCollider(std::weak_ptr<GameObject> gameObj);
//		virtual ~RayCollider() = default;
//
//		void Release() override;
//
//	private:
//		// �� �ݶ��̴� ���� �浹 ����
//		bool CheckForBox(std::shared_ptr<BoxCollider> other);
//		bool CheckForSphere(std::shared_ptr<SphereCollider> other);
//
//
//	public:
//		const Vector3& GetLocalCenter() { return localCenter; }
//		const Vector3& GetWorldCenter() { return worldCenter; }
//		const Vector3& GetDirection() { return direction; }
//
//		const RaycastHit& GetHit() { return hit; }
//
//		// Hit ����
//		void SetHit();
//		void ClearHit();
//
//		void SetLocalCenter(const Vector3& newCenter) { localCenter = newCenter; }
//		void SetDirection(const Vector3& newDir) { direction = newDir; }
//		// �浹 üũ
//		void InitRenderObj() override {}
//		void ColliderRender() override {}
//
//		//void ColliderRender() override;
//
//		void LateUpdate(float tick) override;
//
//		friend class PhysicsManager;
//		friend class SphereCollider;
//		friend class BoxCollider;
//	};
//
//}

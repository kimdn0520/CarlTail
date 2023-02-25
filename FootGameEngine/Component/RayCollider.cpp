#include "GamePch.h"
#include "RayCollider.h"

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Object/GameObject.h"
#include "Transform.h"
#include <utility>
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	RayCollider::RayCollider(std::weak_ptr<GameObject> gameObj)
		: Collider(gameObj)
		, localCenter(Vector3::Zero)
		, worldCenter(transform->GetWorldPosition())
		, direction(transform->GetLook())
		, hit({})
	{
		// ���� ���Ϳ��� ���� Dir�������� ������..
		Vector4 vertices[2] =
		{
			{0.f, 0.f, 0.f, 1.0f},
			{(direction.x * 10.f), (direction.y * 10.f), (direction.z * 10.f), 1.0f}
		};

		WORD indices[2]
		{
			0, 1
		};

		// �׷��Ƚ� ���� �����;���.
		/*Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("RayLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 2);
		factory->CreateIndexBuffer("RayLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 2);*/
	}

	void RayCollider::Release()
	{
		__super::Release();
	}

	bool RayCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		
		return true;
	}

	bool RayCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		
		return true;
	}

	void RayCollider::SetHit()
	{
		// ������ ���ٸ�..
		// ���� hit �� �༮�� �Ÿ��� �� ������ => ���� �տ� �ִ�.
		/*if ((hit.hitName != TEXT("HitNone") && hit.hitDist < dist) || dist < 0)
			return;

		hit.hitGameObject = gameObj;
		hit.hitTransform = gameObj->GetComponent<Transform>();
		hit.hitDist = dist;
		hit.hitName = gameObj->GetName();*/

		if (currHitObjVec.empty() == true)
			return;

		int idx = 0;
		float min = FLT_MAX;

		for (int i = 0; i < currHitObjVec.size(); i++)
		{
			if (min > currHitObjVec[i].second && currHitObjVec[i].second > 0)
			{
				min = currHitObjVec[i].second;
				idx = i;
			}
		}

		hit.hitGameObject = currHitObjVec[idx].first;
		hit.hitTransform = currHitObjVec[idx].first->GetComponent<Transform>();
		hit.hitDist = currHitObjVec[idx].second;
		hit.hitName = currHitObjVec[idx].first->GetName();
	}

	void RayCollider::ClearHit()
	{
		// Hit �ʱ�ȭ.
			// ���� ���� �ʱ�ȭ
		currHitObjVec.clear();
	}

	/*void RayCollider::ColliderRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		Matrix lookAt = MatrixLookAt(worldCenter, direction, Vector::UnitY);

		Matrix world = MatrixInverse(lookAt);

		renderer->GraphicsDebugBeginEvent("Ray Line");
		renderer->DrawLine(resourceManager->GetBuffer("RayLineVB"), resourceManager->GetBuffer("RayLineIB"), color, MatrixTranspose(world));
		renderer->GraphicsDebugEndEvent();

	}*/

	void RayCollider::LateUpdate(float tick)
	{
		// ������Ʈ ó��
		// ���� ���� ������Ʈ
		worldCenter = localCenter + transform->GetWorldPosition();

		// direction ����
		//direction = transform->GetLook();
	}
}

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
		: ColliderBase(gameObj)
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
		// this�� Ray�� other�� Ray�� �ƴ� ��.
		Matrix obbRotInv = other->objWorldRotTM.Invert();

		// �������� other�� �����̽��� ��ȯ
		Vector3 rayLocalCenter = Vector3::Transform(worldCenter, obbRotInv);
		// direction�� ��ȯ���ֱ�
		Vector3 rayLocalDir = Vector3::Transform(direction, obbRotInv);
		rayLocalDir.Normalize(rayLocalDir);
		rayLocalDir = Vector3{ 1.0f / rayLocalDir.x, 1.0f / rayLocalDir.y, 1.0f / rayLocalDir.z };

		Vector3 boxLocalCenter = Vector3::Transform(other->worldCenter, obbRotInv);

		// Box�� ���ؽ� �� x, y, z�� �ּ�, �ִ� ��.
		Vector3 boxMin = { FLT_MAX, FLT_MAX, FLT_MAX };
		Vector3 boxMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		// ������ ��� ���ؽ� ��ǥ�� ���鼭 MinMax�� �̾Ƴ���..
			// extents�� Ȱ���ؼ� ��������.
		std::vector<Vector3> vertices;
		// �߽ɿ��� �� extents ���� �����ְ� ���ָ� vertices ������ ���´�.
			// extents == �� ���� half
		Vector3 tempVertex = { boxLocalCenter.x + other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x + other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x + other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x + other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);

		for (int i = 0; i < vertices.size(); i++)
		{

			// ���ؽ��� �� �� ��ǥ
			float vertexCoord = vertices[i].x;

			if (boxMin.x > vertexCoord)
				boxMin.x = vertexCoord;

			if (boxMax.x < vertexCoord)
				boxMax.x = vertexCoord;

			vertexCoord = vertices[i].y;

			if (boxMin.y > vertexCoord)
				boxMin.y = vertexCoord;

			if (boxMax.y < vertexCoord)
				boxMax.y = vertexCoord;

			vertexCoord = vertices[i].z;

			if (boxMin.z > vertexCoord)
				boxMin.z = vertexCoord;

			if (boxMax.z < vertexCoord)
				boxMax.z = vertexCoord;
		}

		// ������ ��ġ�� �̾Ƴ� ����
		boxMin = boxMin - rayLocalCenter;
		boxMax = boxMax - rayLocalCenter;

		// ���̿� ����
		Vector3 t1 = boxMin * rayLocalDir;
		Vector3 t2 = boxMax * rayLocalDir;

		// �� ��ǥ���� �ּ� �ִ밪.
		Vector3 tMin;
		Vector3 tMax;

		if (t1.x > t2.x)
		{
			tMin.x = t2.x;
			tMax.x = t1.x;
		}
		else
		{
			tMin.x = t1.x;
			tMax.x = t2.x;
		}

		if (t1.y > t2.y)
		{
			tMin.y = t2.y;
			tMax.y = t1.y;
		}
		else
		{
			tMin.y = t1.y;
			tMax.y = t2.y;
		}

		if (t1.z > t2.z)
		{
			tMin.z = t2.z;
			tMax.z = t1.z;
		}
		else
		{
			tMin.z = t1.z;
			tMax.z = t2.z;
		}

		float maxOfMin = -FLT_MAX;
		float minOfMax = FLT_MAX;

		// Min���� ���� ū ���� Max���� ���� ���� ���� ã�´�.
		if (maxOfMin < tMin.x)
			maxOfMin = tMin.x;

		if (minOfMax > tMax.x)
			minOfMax = tMax.x;

		if (maxOfMin < tMin.y)
			maxOfMin = tMin.y;

		if (minOfMax > tMax.y)
			minOfMax = tMax.y;

		if (maxOfMin < tMin.z)
			maxOfMin = tMin.z;

		if (minOfMax > tMax.z)
			minOfMax = tMax.z;


		if (maxOfMin > minOfMax)
			return false;


		if (minOfMax < 0.0f)
			return false;


		//SetHit(other->GetGameObject(), maxOfMin);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), maxOfMin));

		return true;
	}

	bool RayCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// ���� �� �ִ� ���� ��ġ
		float t0, t1;

		// ���� �߽����� ���ϴ� ����
		Vector3 rayToSphere = other->GetWorldCenter() - worldCenter;

		// �ش� ���͸� dir�� �翵.
		float projRayToSphere = rayToSphere.Dot(direction);

		// ���� �߽ɿ��� ���� ���� ������ ������ ����
		float d = rayToSphere.Dot(rayToSphere) - projRayToSphere * projRayToSphere;

		// �� ���̰� ���������� ũ��. ������ �ʴ´�.
		if (d > other->radius)
			return false;

		// �߽ɿ��� ���ͷ� ���� �������� ���� �ε����� �κб����� �Ÿ�
		float solDist = sqrtf(other->radius - d);
		t0 = projRayToSphere - solDist;
		t1 = projRayToSphere + solDist;

		// t0�� t1���� ũ�� -> �� �ڿ� ������.. �����Ѵ�.
		if (t0 > t1)
			std::swap(t0, t1);

		// ��� 0���� ������ t1�� ��� ����.
		if (t0 < 0)
		{
			t0 = t1;
			// �׷����� 0���� ������. false�� �����Ѵ�.
			if (t0 < 0)
				return false;
		}

		// �� �Ÿ��� �־��ش�.
		//SetHit(other->GetGameObject(), t0);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), t0));

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

	bool RayCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// ����.. �ڽ��� ���ش�.
		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;

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

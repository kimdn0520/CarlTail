#include "GamePch.h"
#include "BoxCollider.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Object/GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include <utility>

#include "Component/Camera.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "SceneBase.h"

namespace GameEngineSpace
{
	BoxCollider::BoxCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, size(Vector3{ 1.f, 1.f, 1.f })
		, localCenter(Vector3::Zero)
		, worldCenter(transform->GetWorldPosition())
		, colliderRenderObj(nullptr)
	{
		objWorldRotTM = Matrix::Identity;
		worldNormals = std::vector<Vector3>(3);
		worldNormals[0] = Vector3{ 1.f, 0.f, 0.f };	// Vector3::Right
		worldNormals[1] = Vector3{ 0.f, 1.f, 0.f };	// Vector3::Up
		worldNormals[2] = Vector3{ 0.f, 0.f, 1.f };	// Vector3::BackWard;

		extents = std::vector<float>(3, size.x * 0.5f);

		// �⺻���� ���ؽ� ���۸� ����ϴ�
			// �ش� ������ ������ ���� ���� TM���� �������ݴϴ�.
			// ���⼭�� �ܼ��� 1.f 1.f 1.f�� ���ڸ� �׷��ݴϴ�.
			// �⺻������ 8���� ���ؽ� (�ؽ��İ� ���� ���� -> �ɰ����� �ʿ䰡 ����..
		/*Vector4 vertices[8] =
		{
			// upSide
			{-extents[0], extents[1], extents[2], 1.0f},
			{extents[0], extents[1], extents[2], 1.0f},
			{extents[0], extents[1], -extents[2], 1.0f},
			{-extents[0], extents[1], -extents[2], 1.0f},

			// downside
			{-extents[0], -extents[1], extents[2], 1.0f},
			{extents[0], -extents[1], extents[2], 1.0f},
			{extents[0], -extents[1], -extents[2], 1.0f},
			{-extents[0], -extents[1], -extents[2], 1.0f}
		};

		WORD indices[24] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			0, 4,
			1, 5,
			2, 6,
			3, 7,
			4, 5,
			5, 6,
			6, 7,
			7, 4
		};*/

		// �׷��Ƚ� �������� ���� ����
		/*Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("BoxLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 8);
		factory->CreateIndexBuffer("BoxLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 24);*/
	}

	BoxCollider::~BoxCollider()
	{
	}

	void BoxCollider::Release()
	{
		__super::Release();
	}

	// OBB�� �̿��Ѵ�. by Comet
	bool BoxCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		const Vector3& otherCenter = other->worldCenter;
		// �� ������� �Ÿ��� �޾ƿ´�.
		const std::vector<float>& otherExtents = other->extents;
		// ������ �ʿ��� ����� �޾ƿ´�.
		const std::vector<Vector3>& otherNormals = other->worldNormals;

		// ���� ������ ����� �����ϴ� ������.
		float otherCDotPlane = 0.f;
		float myCDotPlane = 0.f;
		float otherDotRadius = 0.f;
		float myDotRadius = 0.f;
		float otherDotMin = 0.f;
		float otherDotMax = 0.f;
		float myDotMin = 0.f;
		float myDotMax = 0.f;

		// �� ���鿡 ���� �˻�.
			// ������ü�� �������� ���� ���� �� ���̱� ������ �� ���� ���ָ� �ȴ�.
		for (size_t i = 0; i < 3; i++)
		{
			const Vector3& targetNormal = worldNormals[i];

			// �߽ɿ��� �Ǻ� ���� ���� ����.
			otherCDotPlane = otherCenter.Dot(targetNormal);
			myCDotPlane = worldCenter.Dot(targetNormal);

			otherDotRadius = 0.f;

			// �и��� �������� ã�� ����
				// TODO �ش� ������ ��������..
				// �� ���� �븻�� ���̸� ���ؼ����� �༮�� ��� ������ Radius..
				// �ᱹ �Ǻ��࿡���� ���̸� �����ִ� �����̴�.
			for (size_t j = 0; j < 3; j++)
				otherDotRadius += fabsf(otherNormals[j].Dot(targetNormal)) * otherExtents[j];

			myDotRadius = extents[i];

			otherDotMin = otherCDotPlane - otherDotRadius;
			otherDotMax = otherCDotPlane + otherDotRadius;

			myDotMin = myCDotPlane - myDotRadius;
			myDotMax = myCDotPlane + myDotRadius;

			// �и��� �߰�.
				// �ش� �࿡�� ��ġ�� �� ������..!
			if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
				return false;
		}

		// ����� ��鿡 ���� �˻�
		for (size_t i = 0; i < 3; i++)
		{
			const Vector3& targetNormal = otherNormals[i];

			otherCDotPlane = otherCenter.Dot(targetNormal);
			myCDotPlane = worldCenter.Dot(targetNormal);

			myDotRadius = 0.f;

			for (size_t j = 0; j < 3; j++)
				myDotRadius += fabsf(worldNormals[j].Dot(targetNormal)) * extents[j];

			otherDotRadius = otherExtents[i];

			otherDotMin = otherCDotPlane - otherDotRadius;
			otherDotMax = otherCDotPlane + otherDotRadius;

			myDotMin = myCDotPlane - myDotRadius;
			myDotMax = myCDotPlane + myDotRadius;

			// �и��� �߰�.
				// �ش� �࿡�� ��ġ�� �� ������..!
			if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
				return false;
		}

		// �� �𼭸��� �������� �����Ǵ� ��鿡 ���� �˻�.
			// �����θ� ���������� ó���� �ȵǴ� �浹 ������ �˻��ϱ� ����.
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				Vector3 targetNormal;
				worldNormals[i].Cross(otherNormals[j], targetNormal);

				if (targetNormal == Vector3::Zero)
					continue;

				targetNormal.Normalize(targetNormal);

				otherCDotPlane = otherCenter.Dot(targetNormal);
				myCDotPlane = worldCenter.Dot(targetNormal);

				myDotRadius = 0.f;

				for (size_t k = 0; k < 3; k++)
					myDotRadius += fabsf(worldNormals[k].Dot(targetNormal)) * extents[k];

				otherDotRadius = 0.f;

				for (size_t k = 0; k < 3; k++)
					otherDotRadius += fabsf(otherNormals[k].Dot(targetNormal)) * otherExtents[k];

				otherDotMin = otherCDotPlane - otherDotRadius;
				otherDotMax = otherCDotPlane + otherDotRadius;

				myDotMin = myCDotPlane - myDotRadius;
				myDotMax = myCDotPlane + myDotRadius;

				if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
					return false;
			}
		}

		//DoNotCheckEdge:

		// ��� ��쿡 �и����� ã�� ���ߴٸ�? �̰� �浹�̴�!
		return true;
	}

	bool BoxCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// ������ ���� �� �Ͱ� ����������.. obb�� AABB �����̽��� �ű� ��. �浹 üũ�Ѵ�.
		Matrix obbRotInv = objWorldRotTM.Invert();

		Vector3 sphereLocalCenter = Vector3::Transform(other->worldCenter, obbRotInv);
		Vector3 boxLocalCenter = Vector3::Transform(worldCenter, obbRotInv);

		Vector3 boxMin = std::move(boxLocalCenter - size * 0.5);
		Vector3 boxMax = std::move(boxLocalCenter + size * 0.5);

		float minX = std::max<float>(boxMin.x, std::min<float>(sphereLocalCenter.x, boxMax.x));
		float minY = std::max<float>(boxMin.y, std::min<float>(sphereLocalCenter.y, boxMax.y));
		float minZ = std::max<float>(boxMin.z, std::min<float>(sphereLocalCenter.z, boxMax.z));

		Vector3 minPos = Vector3{ minX, minY, minZ };

		float minDist = (minPos - sphereLocalCenter).Length();

		if (minDist <= other->radius)
			return true;
		else
			return false;
	}

	void BoxCollider::SetSize(const Vector3& newSize)
	{
		size = newSize;
		tmpSize = newSize;

		extents[0] = size.x * 0.5;
		extents[1] = size.y * 0.5;
		extents[2] = size.z * 0.5;

	}

	bool BoxCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// ���� ĳ���� ����..

		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;
	}

	void BoxCollider::InitRenderObj(std::string _objName)
	{
		colliderRenderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
		BuilderManger::GetInstance()->GetBuilder("LineBuilder"), _objName);

		// �ش� Obj�� ���ӿ��� ���ο��� Debug������ ���� �״ٸ� �� �����̹Ƿ� ���� ����ť�� �־����� �ʴ´�.
		auto renderer = GraphicsManager::GetInstance()->GetRenderer();
		renderer->InitObject(colliderRenderObj);
	}

	void BoxCollider::ColliderRender()
	{
		auto renderer = GraphicsManager::GetInstance()->GetRenderer();

		Matrix worldPos = XMMatrixTranslation(worldCenter.x, worldCenter.y, worldCenter.z);
		Matrix worldScale = XMMatrixScaling(size.x, size.y, size.z);
		Matrix worldTM = worldScale * objWorldRotTM * worldPos;

		auto cam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		colliderRenderObj->Update(worldTM, cam->GetViewMatrix(), cam->GetProjMatrix());
		renderer->GraphicsDebugBeginEvent("Box Line");
		colliderRenderObj->Render();
		renderer->GraphicsDebugEndEvent();
	}

	void BoxCollider::LateUpdate(float tick)
	{
		// ���� ���� ������Ʈ
		worldCenter = localCenter + transform->GetWorldPosition();

		size = tmpSize * transform->GetWorldScale();

		Quaternion tmpRotQuat = XMQuaternionRotationRollPitchYawFromVector(transform->GetWorldRotation());

		Matrix objRotTM = XMMatrixRotationQuaternion(transform->EulerToQuat(transform->GetWorldRotation()));

		// ���� �� �����Ӱ� ���ؼ� �̹� �����ӿ� ȸ�� ���� ��ȭ�� ���� ���.
		if (objWorldRotTM == objRotTM)
			return;

		// �浹 �˻翡 �ʿ��� ������ ĳ��
		objWorldRotTM = objRotTM;

		// Transform
		worldNormals[0] = Vector3{ 1.f, 0.f, 0.f };
		worldNormals[0] = Vector3::Transform(worldNormals[0], objRotTM);
		worldNormals[0].Normalize(worldNormals[0]);

		worldNormals[1] = Vector3{ 0.f, 1.f, 0.f };
		worldNormals[1] = Vector3::Transform(worldNormals[1], objRotTM);
		worldNormals[1].Normalize(worldNormals[1]);

		worldNormals[2] = Vector3{ 0.f, 0.f, 1.f };
		worldNormals[2] = Vector3::Transform(worldNormals[2], objRotTM);
		worldNormals[2].Normalize(worldNormals[2]);
	}
}

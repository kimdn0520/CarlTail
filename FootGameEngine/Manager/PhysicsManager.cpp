#include "GamePch.h"
#include "PhysicsManager.h"

#include "Object/GameObject.h"
#include "Component/ColliderBase.h"
#include "Component/RayCollider.h"

namespace GameEngineSpace
{
	std::shared_ptr<PhysicsManager> PhysicsManager::instance = nullptr;

	std::shared_ptr<PhysicsManager> PhysicsManager::GetInstance()
	{
		if(instance == nullptr)
			instance = std::make_shared<PhysicsManager>();

		return instance;
	}

	void PhysicsManager::Init()
	{
		// TODO ��ȹ �и� ������ ó��.
	}

	void PhysicsManager::Update(float deltaTime, std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// ������Ʈ ��. ���� �������� �ʱ�ȭ.
		ResetColliders(colliderList);

		// �浹 üũ
		CheckCollisionAll(colliderList);

		// ���� üũ�� ���� ��ε�ĳ��Ʈ
		BroadCastTriggerEvents(colliderList);
	}

	void PhysicsManager::Release()
	{
		// TODO ���� ��ȹ ������.
	}

	void PhysicsManager::CheckCollisionAll(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// ��� ������ ���鼭 �浹 ó�� ���ش�.
		// ������ �ƹ� ���� ���� n^2���� ���� ���Ұ� �浹 ó�� ������ �͵鸸 ó������ �ʿ䰡 �ִ�.
		// TODO ���� ����, ����ȭ �ݵ�� �ʿ�.

		for(size_t i = 0; i < colliderList.size(); i++)
		{
			for(size_t j = i + 1; j < colliderList.size(); j++)
			{
				std::shared_ptr<ColliderBase> collider = colliderList[i];
				std::shared_ptr<ColliderBase> otherCollider = colliderList[j];

				// �ݶ��̴��� ��밡������ ������.
				if(collider->GetIsEnable() == false || otherCollider->GetIsEnable() == false)
					continue;

				// �� �ݶ��̴��� ���� ���ӿ�����Ʈ �ȿ� ������.. Continue
				if(collider->GetGameObject() == otherCollider->GetGameObject())
					continue;

				// �浹�� �� �θ�� continue;
					// ���� ������ �θ�� or ������ ���� �θ��..
				if(collider->GetGameObject() == otherCollider->GetGameObject()->GetParent()
				|| collider->GetGameObject()->GetParent() == otherCollider->GetGameObject())
					continue;
					
				if(collider->CheckCollision(otherCollider))
				{
					// �浹 ������.. ������ �ݶ��̴��� �浹�� ��ü ���Ϳ� other�� �־��ݴϴ�.
					collider->currColliedColList.push_back(otherCollider);
					otherCollider->currColliedColList.push_back(collider);

					// ����׿� �ڵ� => ���� �浹 �ϰ� �ִ°�..
					collider->SetColor({1.0f, 0.f, 0.f});
					otherCollider->SetColor({1.0f, 0.f, 0.f});

					/*TCHAR buffer[256] = {};*/

					//_stprintf_s<256>(buffer, _T("Colided : %s <-> %s\n"), collider->GetGameObject()->GetName().c_str(), otherCollider->GetGameObject()->GetName().c_str());
					//OutputDebugString(buffer);
				}
			}
		}
	}

	void PhysicsManager::ResetColliders(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		for(auto& collider : colliderList)
		{
			// ���鼭 ����
			// ���� �����ӿ� �ִ� �ݶ��̴� ���� ���� ���������� �Ѱ��ְ�.
			collider->prevColliedColList = collider->currColliedColList;

			// ���� �������� Ŭ�������ش�.
			collider->ClearCurrColliedColList();

			std::shared_ptr<RayCollider> ray = std::dynamic_pointer_cast<RayCollider>(collider);
			// ���̸� Hit�� Ŭ����.
			if (ray != nullptr)
				ray->ClearHit();
		}
	}

	void PhysicsManager::BroadCastTriggerEvents(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// �ݶ��̴� ��ü�� ���鼭.. Ʈ���Ÿ� üũ���ݴϴ�.
		for(auto& collider : colliderList)
		{
			// ���� �ݶ��̴��� �̹� �����ӿ� �浹�� �ݶ��̴�
			std::vector<std::shared_ptr<ColliderBase>>& currColList = collider->currColliedColList;

			// �ݶ��̴��� ray�� continue
			std::shared_ptr<RayCollider> ray = std::dynamic_pointer_cast<RayCollider>(collider);

			// ���� �浹�Ѱ� ���� ��.. �� ����
			if (currColList.size() == 0)
			{
				collider->SetColor({ 1.0f, 1.0f, 1.0f });
			}

			if(ray != nullptr)
			{
				// ���̸� ���⼭ SetHit
				ray->SetHit();
				continue;
			}

			// ���� �ݶ��̴��� ������ �����ӿ� �浹�� �ݶ��̴�
			std::vector<std::shared_ptr<ColliderBase>>& prevColList = collider->prevColliedColList;

			std::shared_ptr<GameObject> gameObject = collider->GetGameObject();

			// �ΰ��� ���� ���鼭..
				// ���� �����ӿ� �浹�� �ݶ��̴�
			for(auto& currCollider : currColList)
			{
				// �ݶ��̴��� ray�� continue
				ray = std::dynamic_pointer_cast<RayCollider>(currCollider);

				if (ray != nullptr)
					continue;

				// ���� �����ӿ� �浹�� �ִµ� ���Ű� ������ Enter. �Ѵ� ������ stay
				if(std::find(prevColList.begin(), prevColList.end(), currCollider) != prevColList.end())
					gameObject->OnTriggerStay(currCollider);
				else
					gameObject->OnTriggerEnter(currCollider);
			}

			// �ݴ�� ���翡 ���µ� ���ſ� ������..? Exit
			for(auto& prevCollider : prevColList)
			{
				// �ݶ��̴��� ray�� continue
				ray = std::dynamic_pointer_cast<RayCollider>(prevCollider);

				if (ray != nullptr)
					continue;

				// ���� �ݶ��̴� ����Ʈ�� ���ۺ��� ������ ������.. ������ �ݶ��̴� ����Ʈ�� �ִ� �ְ� ������..!
					// �갡 �̹� �����ӿ� ��������..!
				if(std::find(currColList.begin(), currColList.end(), prevCollider) == currColList.end())
					gameObject->OnTriggerExit(prevCollider);
			}
		}
	}
}

#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class ColliderBase;

	/**
	 * \brief CometEngine�� PhysicsManager�� ����� physicsManager
	 *
	 * �浹, ������ ���õ� ó���� ������.
	 * ����� ������ �浹 ó���� ����.
	 *
	 * 2022.08.03(��)
	 */
	class PhysicsManager final
	{
	private:
		// �ν��Ͻ��� ������� ������ �Ŵ���..
		static std::shared_ptr<PhysicsManager> instance;

	public :
		PhysicsManager() = default;
		~PhysicsManager() = default;
		PhysicsManager(const PhysicsManager& other) = delete;
		PhysicsManager(const PhysicsManager&& other) = delete;
		PhysicsManager& operator= (const PhysicsManager& other) = delete;
		PhysicsManager& operator= (const PhysicsManager&& other) = delete;

		static std::shared_ptr<PhysicsManager> GetInstance();

		void Init();

		// ���͸� ���۷�����..
		void Update(float deltaTime, std::vector<std::shared_ptr<ColliderBase>>& colliderList);

		void Release();

	private:
		// �浹 üũ.
		void CheckCollisionAll(std::vector<std::shared_ptr<ColliderBase>>& colliderList);

		// �ݶ��̴����� ����
		void ResetColliders(std::vector<std::shared_ptr<ColliderBase>>& colliderList);

		// ������ �����ش�.
			// -> onTrigger, ExitTrigger
		void BroadCastTriggerEvents(std::vector<std::shared_ptr<ColliderBase>>& colliderList);

	};

}

#pragma once
#include <mutex>
#include "PxSimulationEventCallback.h"

namespace MoolyEngine
{
	class Scene;

	class EventCallback : public physx::PxSimulationEventCallback
	{
	public:
		EventCallback() = default;
		~EventCallback();

		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {};
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {};

		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider*);
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger*);

		void DeleteEvents(std::string eventHolder);

		void CallonTriggerPersist();

	private:
		std::weak_ptr<Scene> m_Scene;

		//void(Ÿ�� ������Ʈ �̸�)��ȯ. �Լ��� ���ε���. �̸��� �Ȱ��� ����� �Լ��� �ҷ��´�.
		std::unordered_map<std::string, std::vector<IEventCollider*>> contactEvents;
		//Ʈ���Ÿ� ����
		std::unordered_map<std::string, std::vector<IEventTrigger*>> triggerEvents;
		std::unordered_map<std::string, std::list<std::string>> triggerPersistEvents;

		std::mutex mutex;
	};
}
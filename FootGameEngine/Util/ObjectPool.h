#pragma once
#include "Object/GameObject.h"

namespace GameEngineSpace
{
	class ObjectPool
	{
	public:
		ObjectPool();

		~ObjectPool();

		void RegisterGameObject(std::string name, int size, std::function<std::shared_ptr<GameObject>()> gameObjectFunction);

		std::shared_ptr<GameObject> AcquireGameObject(std::string name);

		void ReturnGameObject(std::shared_ptr<GameObject> gameObject);

		// ���� �ٲ𶧸��� �ʱ�ȭ ������
		void Release();

	private:
		std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> pool;
	};
}

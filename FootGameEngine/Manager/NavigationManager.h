#pragma once

namespace GameEngineSpace
{
	static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
	static const int NAVMESHSET_VERSION = 1;

	struct NavMeshSetHeader
	{
		int magic;
		int version;
		int numTiles;
		dtNavMeshParams params;
	};

	struct NavMeshTileHeader
	{
		dtTileRef tileRef;
		int dataSize;
	};

	/**
	 * \brief Navigation�� �����ϴ� Ŭ����
	 *
	 * �ش� �Ŵ��� Ŭ������ ������ ���ӿ������� ������ �ִ�. �̱������� �����Ѵ�.
	 *
	 * 2023.01.08(��) Dev.Newbie
	 */
	class NavigationManager final
	{
		// �� �ϳ� �����ϴ� SceneManager ��ü
		static std::shared_ptr<NavigationManager> instance;

	private:

	public:
		NavigationManager() = default;

		~NavigationManager() = default;

		// �̵�, ���� ������ ����
		NavigationManager(const NavigationManager& _other) = delete;
		NavigationManager& operator= (const NavigationManager& _other) = delete;
		NavigationManager(NavigationManager&& _other) = delete;
		NavigationManager& operator= (NavigationManager&& _other) = delete;

		static std::shared_ptr<NavigationManager> GetInstance();

		void Release();

		void MakeNavMesh(std::string navMeshName);
	};
}


#pragma once
#include "FBXBinaryLayout.h"

namespace GameEngineSpace
{
	class GameObject;

	// �ʿ信 ���� �̷��� ����� �����ؾ��Ѵ�.
	enum PolyAreas
	{
		POLYAREA_GROUND,
		POLYAREA_WATER,
		POLYAREA_ROAD,
		POLYAREA_DOOR,
		POLYAREA_GRASS,
		POLYAREA_JUMP
	};

	enum PolyFlags
	{
		POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
		POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
		POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
		POLYFLAGS_JUMP = 0x08,		// Ability to jump.
		POLYFLAGS_DISABLED = 0x10,	// Disabled polygon
		POLYFLAGS_ALL = 0xffff		// All abilities.
	};

	constexpr const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
	constexpr const int NAVMESHSET_VERSION = 1;

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
	 * \brief Navigation.bin�� �ҷ��ͼ� �����ϴ� Ŭ����
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
		bool isDebugRender = false;

		//std::shared_ptr<GameObject> nav;

		std::vector<FBXBinaryData::VertexData> vertexDataList;
		std::vector<std::vector<unsigned int>> indices;
		unsigned int idx_offset = 0;

		bool isNavMesh = false;

	public:
		dtNavMesh* navMesh;
		dtNavMeshQuery* navQuery;
		dtQueryFilter filter;				// ������ ������ ������ ����

		std::string currentNavMeshName = "";

	public:
		NavigationManager() = default;

		~NavigationManager() = default;

		// �̵�, ���� ������ ����
		NavigationManager(const NavigationManager& _other) = delete;
		NavigationManager& operator= (const NavigationManager& _other) = delete;
		NavigationManager(NavigationManager&& _other) = delete;
		NavigationManager& operator= (NavigationManager&& _other) = delete;

		static std::shared_ptr<NavigationManager> GetInstance();
	
	public:
		void Release();

		// NavMesh�� �ҷ��´�.
		void MakeNavMesh(std::string navMeshName);

		// NavMesh �� �׷��ְ� �ʹ�. �����ұ�..?
		void DebugRenderNavMesh();
	};
}


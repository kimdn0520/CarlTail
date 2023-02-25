#pragma once
#include "GraphicsCore/RasterizerState.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "YAMLBinaryLayout.h"
#include "FBXBinaryLayout.h"

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;

	class UnityScene;

	// ���� �������� ��� �����ϵ��� DLL Export�� ���ش�.
	class GRAPHICSENGINE_DECLSPEC ResourceManager
	{
	private:
		static std::shared_ptr<ResourceManager> instance;

	public:
		ResourceManager()
		{}
		~ResourceManager() {};

		static std::shared_ptr<ResourceManager> GetInstance();

	private:
		std::unordered_map<std::string, std::shared_ptr<IDXObject>> prefab;

	private:
		std::string meshPath;
		std::string materialPath;
		std::string AnimationPath;
		std::string modelPath;
		std::string prefabPath;

	public:
		// ���ο��� ��� ���� �⺻ �޽õ�(����, navmesh)
		std::unordered_map<std::string, std::shared_ptr<Mesh>> basicMeshes;

	public:
		std::shared_ptr<Mesh> LoadMesh(std::string name);

		std::shared_ptr<Texture> LoadTexture(std::wstring path);

	public:
		// �޽ø� ���� �Ѱ��ش�.
		template <typename T>
		const std::shared_ptr<Mesh> CreateMesh(std::vector<T> vertices, std::vector<std::vector<unsigned int>> indices, std::string name, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState);
		
		void Initialize();

		void Release();

		void LoadCubeMesh();			

		void LoadSphereMesh();			

		void LoadAxisMesh();			

		void LoadGridMesh();			

		void LoadScreenMesh();			

		void LoadDebugScreenMesh();		

		void LoadWireCubeMesh();		

		void LoadWireSphereMesh();

		void LoadCapsuleMesh();

		void LoadPlaneMesh();			

		void LoadPrefab();

		// ���ӿ��� �ʿ��� mesh�� ���� ���� �� �ֵ��� ���ִ� �Լ�
		void MakeMesh(std::string name, std::vector<FBXBinaryData::VertexData> vertex, std::vector<std::vector<unsigned int>> indices);

		std::shared_ptr<YAMLBinaryData::Scene> LoadUnityScene(std::string path);
	};

	template<typename T>
	inline const std::shared_ptr<Mesh> ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<std::vector<unsigned int>> indices, std::string name, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState)
	{
		// TODO : unique�� �´°�?
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		mesh->SetName(name);

		mesh->SetPrimitiveTopology(topology);
		
		mesh->SetRenderState(rasterizerState);

		if (!vertices.empty())
		{
			mesh->CreateVertexBuffer(vertices);
			for (int i = 0; i < indices.size(); i++)
			{
				mesh->CreateIndexBuffer(indices[i]);
			}
			mesh->SetStride(sizeof(T));
		}

		return mesh;
	}
}
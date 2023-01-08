#include "GamePch.h"
#include "SceneBase.h"

#include "Object/GameObject.h"
#include "Component/RendererBase.h"
#include "GraphicsManager.h"

#include "Timer.h"
#include "UIManager.h"

#include "Component/ColliderBase.h"

#include "Component/Camera.h"
#include "InputManager.h"
#include "GraphicsManager.h"


namespace GameEngineSpace
{
#ifdef _DEBUG
	bool SceneBase::debugRender = true;
#else
	bool SceneBase::debugRender = false;
#endif

	SceneBase::SceneBase(tstring sceneName) :
		sceneName(sceneName)
		, mainCamera(nullptr)
		, gameObjectInScene(std::vector<std::shared_ptr<GameObject>>())
		, renderObjInScene(std::vector<std::shared_ptr<RendererBase>>())
		, colliderObjInScene(std::vector<std::shared_ptr<ColliderBase>>())
	{

	}

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::Awake()
	{
		// ���� ���۽�..
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Awake();
		}
	}

	void SceneBase::Start()
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Start();
		}
	}

	void SceneBase::PreUpdate(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->PreUpdate(tick);
		}
	}

	void SceneBase::Update(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Update(tick);
		}
	}

	void SceneBase::LateUpdate(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->LateUpdate(tick);
		}
	}

	void SceneBase::Render(float tick)
	{
		// �� ������Ʈ���� ������ �۾�
		for (auto& renderObj : renderObjInScene)
		{
			// TODO : �ٿ�� ���� ����� �ش� �������� �������� �ø��� ���ݴϴ�.

			if (renderObj->GetIsEnable())
			{
				renderObj->PrepareRender(tick);
			}
		}

		auto renderer = GraphicsManager::GetInstance()->GetRenderer();

		renderer->BeginRender();
		//renderer->DebugRender();

		// �׷��� ���� Collider Render
		for (auto& collider : colliderObjInScene)
		{
			if (collider->GetIsEnable())
				collider->ColliderRender();
		}
		renderer->Render();

		// IMGUI ����
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->DebugIMGUIRender(tick);
		}
		
		UIManager::GetInstance()->SelectGUIRender();

		// Scene�� �� �׷�����, UIManager ����
		UIManager::GetInstance()->Render(tick);

		renderer->EndRender();

	}

	void SceneBase::OnEnable()
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->OnEnable();
		}
	}

	void SceneBase::ClearScene()
	{
		// ���������� �����ش�.
		// ������� �ϳ��� ���� �� ���������� ������
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Release();
		}

		gameObjectInScene.clear();
		renderObjInScene.clear();
		colliderObjInScene.clear();

		// UI �����ֱ�
		//UIManager::GetInstance()->ClearUI();
	}

	void SceneBase::AddGameObject(std::shared_ptr<GameObject> newGameObj)
	{
		gameObjectInScene.push_back(newGameObj);

		// �ڽĵ��� ������ �ڽĵ鵵 �־��ش�.
		const std::vector<std::shared_ptr<GameObject>> children = newGameObj->GetChildren();

		for (auto child : children)
		{
			this->AddGameObject(child);
		}
	}

	std::shared_ptr<GameObject> SceneBase::CreateEmpty()
	{
		// ����� ������Ʈ�� �����.. Ʈ�������� ���� �� �־��ش�.
		std::shared_ptr<GameObject> newGameObj = std::make_shared<GameObject>();

		newGameObj->AddComponent<Transform>();

		AddGameObject(newGameObj);

		// ��ȯ.
		return newGameObj;
	}

	void SceneBase::AddRenderer(std::shared_ptr<RendererBase> renderObj)
	{
		// ���� �� �κ���, �׷��Ƚ����� ����� ��.
		renderObjInScene.push_back(renderObj);
	}

	void SceneBase::AddCollider(std::shared_ptr<ColliderBase> colliderObj)
	{
		colliderObjInScene.push_back(colliderObj);
	}

}

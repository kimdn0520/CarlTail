#include "GamePch.h"
#include "SceneBase.h"

#include "Object/GameObject.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "GraphicsManager.h"
#include "Timer.h"
#include "UIManager.h"

#include "Component/RendererBase.h"
#include "Component/Light.h"
#include "Component/Collider.h"
#include "Component/Camera.h"
#include "NavigationManager.h"

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
		, colliderObjInScene(std::vector<std::shared_ptr<Collider>>())
		, fixedTimeStep(0.01444f)
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

	void SceneBase::FixedUpdate(float tick)
	{
		if (tick >= fixedTimeStep)
		{
			for (auto& gameObj : gameObjectInScene)
			{
				gameObj->FixedUpdate(tick);
			}
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
			if (renderObj->GetIsEnable() == true)
			{
				renderObj->PrepareRender(tick);
			}
		}

		auto renderer = GraphicsManager::GetInstance()->GetRenderer();

		// ShadowRender
		// PointLight ����ŭ �������ô�..
		for (int i = 0; i < pointLightInScene.size(); i++)
		{
			if(pointLightInScene[i]->GetIsShadow() != true)
				continue;

			renderer->ShadowRenderStart(i, true);
			for (auto& renderObj : renderObjInScene)
			{
				if (renderObj->GetIsEnable() == true)
				{
					renderer->ShadowRender(renderObj->GetRenderObj(), true);
				}
			}
			renderer->ShadowRenderEnd();
		}

		// SpotLight Shadow
		for (int i = 0; i < spotLightInScene.size(); i++)
		{
			if (spotLightInScene[i]->GetIsShadow() != true)
				continue;

			renderer->ShadowRenderStart(i, false);
			for (auto& renderObj : renderObjInScene)
			{
				if (renderObj->GetIsEnable() == true)
				{
					renderer->ShadowRender(renderObj->GetRenderObj(), false);
				}
			}
			renderer->ShadowRenderEnd();
		}

		renderer->GraphicsDebugBeginEvent("BeginRender");
		renderer->BeginRender();
		renderer->GraphicsDebugEndEvent();

		// forward���� SkyBox ����
		if (mainCamera != nullptr)
			mainCamera->Render();

		if (debugRender == true)
		{
			// �׷��� ���� Collider Render
			renderer->GraphicsDebugBeginEvent("ColliderRender");
			for (auto& collider : colliderObjInScene)
			{
				if (collider->GetIsEnable() == true)
					collider->ColliderRender();
			}
			renderer->GraphicsDebugEndEvent();
		}

		// Object Render
		renderer->GraphicsDebugBeginEvent("ObjectRender");
		for (auto& renderObj : renderObjInScene)
		{
			if (renderObj->GetIsEnable() == true)
			{
				std::string objName;
				objName.assign(renderObj->GetGameObject()->GetName().begin(), renderObj->GetGameObject()->GetName().end());
				renderer->GraphicsDebugBeginEvent(objName);
				renderObj->Render();
				renderer->GraphicsDebugEndEvent();
			}
		}
		renderer->GraphicsDebugEndEvent();

		// LightPass
		renderer->Render();

		// DebugRender
		DebugRender(tick);

		//UIManager::GetInstance()->SelectGUIRender();
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

		if(objectPool != nullptr)
			objectPool->Release();

		gameObjectInScene.clear();
		renderObjInScene.clear();
		colliderObjInScene.clear();
		pointLightInScene.clear();
		spotLightInScene.clear();

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

	std::shared_ptr<GameObject> SceneBase::InstantiateGameObject(std::shared_ptr<GameObject> _newGameObj)
	{
		std::shared_ptr<GameObject> newGameObj = std::make_shared<GameObject>();

		newGameObj->AddComponent<Transform>();

		AddGameObject(newGameObj);

		newGameObj->Awake();

		newGameObj->Start();

		return newGameObj;
	}

	void SceneBase::DebugRender(float tick)
	{
		auto renderer = GraphicsManager::GetInstance()->GetRenderer();

		// Debug Render
		if (debugRender == true)
		{
			renderer->DebugRender();

			// Debug ��� �ӽ÷� FPS ����
			static float frameTime = 0.0f;
			static int fps = 0;
			static int frameCount = 0;

			frameTime += tick;
			frameCount++;

			if (frameTime >= 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				frameTime = 0.0f;
			}

			auto imgui = ImGUIManager::GetInstance();

			if (imgui->Begin("Main Debug Menu"))
			{
				if (imgui->CollapsingHeader("FPS"))
				{
					imgui->Text("FPS : " + std::to_string(fps));
					imgui->Text("DeltaTime : " + std::to_string(tick));
				}
				imgui->Spacing();

				imgui->CollapsingHeader("Light In Scene");
				imgui->Spacing();

				// IMGUI ����
				for (auto& gameObj : gameObjectInScene)
				{
					gameObj->DebugIMGUIRender(tick);
				}
			}
			imgui->End();

			if (imgui->Begin("Battle Info Menu"))
			{
				// IMGUI ����
				for (auto& gameObj : gameObjectInScene)
				{
					gameObj->DebugIMGUIRender(tick);
				}
			}
			imgui->End();

			// navmesh�� �ִ� scene �̶�� �׷��ش�.
			NavigationManager::GetInstance()->DebugRenderNavMesh();

			UIManager::GetInstance()->SelectGUIRender();
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

	void SceneBase::AddCollider(std::shared_ptr<Collider> colliderObj)
	{
		colliderObjInScene.push_back(colliderObj);
	}

	void SceneBase::AddPointLight(std::shared_ptr<Light> pLight)
	{
		pointLightInScene.push_back(pLight);
	}

	void SceneBase::AddSpotLight(std::shared_ptr<Light> sLight)
	{
		spotLightInScene.push_back(sLight);
	}
}

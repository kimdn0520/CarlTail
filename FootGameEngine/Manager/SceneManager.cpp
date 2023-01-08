#include "GamePch.h"
#include "SceneManager.h"

#include "SceneBase.h"
#include "Object/GameObject.h"
#include "Component/Camera.h"
#include "Component/BoxCollider.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "PhysicsManager.h"
#include "Timer.h"

#include "InputManager.h"
#include "UIManager.h"
#include "GraphicsManager.h"
#include "Component/BasicCameraController.h"

namespace GameEngineSpace
{
	std::shared_ptr<SceneManager> SceneManager::instance = nullptr;

	void SceneManager::LoadSceneData(std::string sceneName)
	{
		auto sceneData = ResourceManager::GetInstance()->LoadUnityScene("BinaryFile/Scene/" + sceneName + ".noob");

		auto scene = SceneManager::GetInstance()->GetCurrentScene();

		for (int i = 0; i < sceneData->gameObjects.size(); i++)
		{
			// ���ӿ�����Ʈ ����
			std::shared_ptr<GameObject> gameObject = scene->CreateEmpty();

			std::wstring gameObjectName;

			gameObjectName.assign(sceneData->gameObjects[i].name.begin(), sceneData->gameObjects[i].name.end());

			// ���ӿ�����Ʈ �̸� ����
			gameObject->SetName(gameObjectName);

			// ���ӿ�����Ʈ fileID ����
			gameObject->SetFileID(sceneData->gameObjects[i].gameObjectID);

			if (sceneData->gameObjects[i].transform != nullptr)
			{
				gameObject->AddComponent<Transform>();

				gameObject->GetComponent<Transform>()->gameObjectID = sceneData->gameObjects[i].transform->gameObjectID;
				gameObject->GetComponent<Transform>()->componentID = sceneData->gameObjects[i].transform->componentID;
				gameObject->GetComponent<Transform>()->childIDList = sceneData->gameObjects[i].transform->childIDList;

				gameObject->GetComponent<Transform>()->SetPosition(Vector3{
					sceneData->gameObjects[i].transform->localPosition.x,
					sceneData->gameObjects[i].transform->localPosition.y,
					sceneData->gameObjects[i].transform->localPosition.z });

				gameObject->GetComponent<Transform>()->SetRotation(Vector3{
					sceneData->gameObjects[i].transform->localRotation.x,
					sceneData->gameObjects[i].transform->localRotation.y,
					sceneData->gameObjects[i].transform->localRotation.z });

				gameObject->GetComponent<Transform>()->SetScale(Vector3{
					sceneData->gameObjects[i].transform->localScale.x,
					sceneData->gameObjects[i].transform->localScale.y,
					sceneData->gameObjects[i].transform->localScale.z });
			}

			if (sceneData->gameObjects[i].camera != nullptr)
			{
				scene->SetMainCamera(gameObject->AddComponent<Camera>());

				gameObject->GetComponent<Camera>()->gameObjectID = sceneData->gameObjects[i].camera->gameObjectID;
				gameObject->GetComponent<Camera>()->componentID = sceneData->gameObjects[i].camera->componentID;

				if (sceneData->gameObjects[i].camera->projectionMatrixMode == 0)
					gameObject->GetComponent<Camera>()->UpdateProjMatrix(ProjType::PERSPECTIVE);
				else
					gameObject->GetComponent<Camera>()->UpdateProjMatrix(ProjType::ORTHOGRAPHIC);

				gameObject->GetComponent<Camera>()->SetNearZ(sceneData->gameObjects[i].camera->nearPlane);

				gameObject->GetComponent<Camera>()->SetFarZ(sceneData->gameObjects[i].camera->farPlane);

				gameObject->AddComponent<BasicCameraController>();

				//gameObject->GetComponent<Transform>()->LookAt(Vector3{ 0.f, 0.f, 0.f });
			}

			if (sceneData->gameObjects[i].light != nullptr)
			{

			}

			if (sceneData->gameObjects[i].boxCollider != nullptr)
			{
				gameObject->AddComponent<BoxCollider>();

				gameObject->GetComponent<BoxCollider>()->gameObjectID = sceneData->gameObjects[i].boxCollider->gameObjectID;
				gameObject->GetComponent<BoxCollider>()->componentID = sceneData->gameObjects[i].boxCollider->componentID;

				gameObject->GetComponent<BoxCollider>()->SetSize(Vector3{
					sceneData->gameObjects[i].boxCollider->size.x,
					sceneData->gameObjects[i].boxCollider->size.y,
					sceneData->gameObjects[i].boxCollider->size.z });

				gameObject->GetComponent<BoxCollider>()->SetLocalCenter(Vector3{
				sceneData->gameObjects[i].boxCollider->center.x,
				sceneData->gameObjects[i].boxCollider->center.y,
				sceneData->gameObjects[i].boxCollider->center.z });

				gameObject->GetComponent<BoxCollider>()->InitRenderObj("WireCube");
			}

			if (sceneData->gameObjects[i].sphereCollider != nullptr)
			{
				/*gameObject->AddComponent<SphereCollider>();

				gameObject->GetComponent<SphereCollider>()->SetRadius(sceneData->gameObjects[i].sphereCollider->radius);

				gameObject->GetComponent<SphereCollider>()->SetLocalCenter(Vector3{
				sceneData->gameObjects[i].sphereCollider->center.x,
				sceneData->gameObjects[i].sphereCollider->center.y,
				sceneData->gameObjects[i].sphereCollider->center.z });*/
			}

			if (sceneData->gameObjects[i].meshFilter != nullptr)
			{
				if (sceneData->gameObjects[i].meshFilter->meshName == "CubeMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("CubeMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "SphereMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("SphereMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "PlaneMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("PlaneMesh");
				}

				// meshrenderer �� meshfilter�κ��� ��������..
				gameObject->GetComponent<MeshRenderer>()->gameObjectID = sceneData->gameObjects[i].meshFilter->gameObjectID;
				gameObject->GetComponent<MeshRenderer>()->componentID = sceneData->gameObjects[i].meshFilter->componentID;
			}
		}

		/// <summary>
		///	�θ� �ִٸ� ���� ������..
		/// </summary>
		auto gameObjectInScene = scene->GetGameObjectInScene();

		for (int i = 0; i < gameObjectInScene.size(); i++)
		{
			if (!gameObjectInScene[i]->GetComponent<Transform>()->childIDList.empty())
			{
				for (auto& childID : gameObjectInScene[i]->GetComponent<Transform>()->childIDList)
				{
					// �ڽ��� �ִ��� ã�´�.
					auto it = find_if(gameObjectInScene.begin(), gameObjectInScene.end(), [&id = childID](const std::shared_ptr<GameObject>& obj)->bool {return (obj->GetComponent<Transform>()->componentID == id); });

					// �ڽ��� ã�Ҵٸ�
					if (it != gameObjectInScene.end())
					{
						gameObjectInScene[i]->SetChild(*it);
					}
				}
			}
		}

		for (int i = 0; i < sceneData->prefabs.size(); i++)
		{
			// ���ӿ�����Ʈ ����
			std::shared_ptr<GameObject> gameObject = scene->CreateEmpty();

			std::wstring gameObjectName;

			gameObjectName.assign(sceneData->prefabs[i].name.begin(), sceneData->prefabs[i].name.end());

			// ���ӿ�����Ʈ �̸� ����
			gameObject->SetName(gameObjectName);

			gameObject->AddComponent<Transform>();

			gameObject->GetComponent<Transform>()->SetPosition(Vector3{
				sceneData->prefabs[i].transform.localPosition.x,
				sceneData->prefabs[i].transform.localPosition.y,
				sceneData->prefabs[i].transform.localPosition.z });

			gameObject->GetComponent<Transform>()->SetRotation(Vector3{
				sceneData->prefabs[i].transform.localRotation.x,
				sceneData->prefabs[i].transform.localRotation.y,
				sceneData->prefabs[i].transform.localRotation.z });

			gameObject->GetComponent<Transform>()->SetScale(Vector3{
				sceneData->prefabs[i].transform.localScale.x,
				sceneData->prefabs[i].transform.localScale.y,
				sceneData->prefabs[i].transform.localScale.z });

			gameObject->AddComponent<MeshRenderer>()->Init(sceneData->prefabs[i].name + ".model");
		}
	}

	void SceneManager::ChangeScene()
	{
		if (currentScene != nullptr)
		{
			// ���� Scene�� ������ �����ش�..
			currentScene->ClearScene();
			// �������� ���� �����ֱ�
				// ��� �̰� ���ҽ� �Ŵ����� ������ �ű⼭ ó�� ������Ѵ�
				// ������ �� �ϳ� ���� ������ �ߺ��Ǵ� ���ҽ� ���� ����
				// �����Ѵٴ� ������ �ִ�.
			GraphicsManager::GetInstance()->GetRenderer()->ClearRenderVector();
		}

		currentScene = scenes.at(reservedLoadSceneName);

		reservedLoadScene = false;

		// �� �ʱ�ȭ.
		currentScene->BuildScene();
		// �� �� ������Ʈ�� init ������
		GraphicsManager::GetInstance()->GetRenderer()->InitObject();
		currentScene->Awake();
		currentScene->Start();
	}

	std::shared_ptr<SceneManager> SceneManager::GetInstance()
	{
		// �ν��Ͻ��� ������ ������ְ� ������ �ν��Ͻ� ����
		if (instance == nullptr)
		{
			instance = std::make_shared<SceneManager>();
		}

		return instance;
	}

	void SceneManager::Init()
	{
		// ���Ŀ� ������ ������ ���⼭ ������ Init���ش�.
		physicsManager = PhysicsManager::GetInstance();

		physicsManager->Init();
	}

	void SceneManager::Release()
	{
		currentScene.reset();

		physicsManager->Release();

		// �� ��ü ������
		for (auto& scene : scenes)
		{
			scene.second->ClearScene();
		}
	}

	void SceneManager::Update()
	{
		// ���� ���� �ִٸ�..
		if (currentScene != nullptr)
		{
			float deltaTime = Timer::GetInstance()->DeltaTime();

			// ��Ÿ Ÿ�� �׽�Ʈ�� �ڵ�
				// ƽ�� �������� -> �Լ��� ���� ����� ���� ����Ѵ�.
			if (isChangedTick == true)
			{
				deltaTime *= deltaMultiple;
				// ���� üũ�� ���ش�.
				if (deltaTime < 0.0f)
					deltaTime = 0.0f;
			}


			// �� ������Ʈ �� �浹 ������Ʈ..
			physicsManager->Update(deltaTime, currentScene->colliderObjInScene);

			// ������� ������Ʈ ���ش�.
			currentScene->PreUpdate(deltaTime);
			currentScene->Update(deltaTime);
			UIManager::GetInstance()->Update();
			currentScene->LateUpdate(deltaTime);

			// ���� ���� => ������ �̵�
			currentScene->Render(deltaTime);

			if (InputManager::GetInstance()->GetInputState(VK_F1, KeyState::DOWN))
			{
				isDebugString = !isDebugString;
				//UIManager::GetInstance()->GetCanvas("DebugCanvas")->SetEnable(isDebugString);
				bool selectMode = UIManager::GetInstance()->GetMainCanvas()->GetSelectMode();
				UIManager::GetInstance()->GetMainCanvas()->SetSelectMode(!selectMode);
			}

			// �׸��� OnEnable..
			currentScene->OnEnable();
		}

		// �� ������ �����Ǿ� ������.. �ٲ��ش�.
		if (reservedLoadScene)
		{
			ChangeScene();
		}
	}

	void SceneManager::AddGameScene(std::shared_ptr<SceneBase> _scene)
	{
		if (_scene != nullptr)
		{
			scenes.insert({ _scene->GetSceneName(), _scene });
		}
	}

	void SceneManager::LoadScene(tstring _sceneName)
	{
		// ���� Map�� �����ϴ°�.
		assert(scenes.find(_sceneName) != scenes.end());

		// ���� �����Ѵ�.
		reservedLoadScene = true;
		reservedLoadSceneName = _sceneName;
	}

	void SceneManager::UpdateMainCameraAspectRatio(uint32 _screenWidth, uint32 _screenHeight)
	{
		if (currentScene != nullptr)
		{
			std::shared_ptr<Camera> _mainCamera = currentScene->GetMainCamera();

			if (_mainCamera != nullptr)
			{
				currentScene->GetMainCamera()->SetAspectRatio(static_cast<float>(_screenWidth) / _screenHeight);
			}
		}
	}

	void SceneManager::ChangeTick(float multiple)
	{
		isChangedTick = true;

		deltaMultiple = multiple;
	}
}

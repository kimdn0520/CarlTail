#include "GamePch.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "../../FootGraphicsEngine/Manager/ResourceManager.h"

#include "SceneManager.h"
#include "Object/GameObject.h"
#include "Component/BoxCollider.h"
#include "Component/Camera.h"
#include "Component/MeshRenderer.h"
#include "SceneBase.h"
#include "UIManager.h"

namespace GameEngineSpace
{
	// ����ƽ���� ����
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// ���⼭ Dll�� �ε����ݴϴ�.
			// TODO : �� ������ �°� �� ����.
#ifdef x64
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Debug_x64.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Release_x64.dll"));
#endif
#else
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Debug_x86.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Release_x86.dll"));
#endif
#endif

		if (graphicsDLL == nullptr)
		{
			// TO DO
			// DLL �ε� ����
			OutputDebugString(_T("Load graphics engine dll failed.\n"));
			return false;
		}

		OutputDebugString(_T("Load graphics engine dll success.\n"));

		// �ʱ� ������� ����� �ϱ����� Ŭ���̾�Ʈ ���� ������.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// �������� �����غ��ô�.
		renderer.reset(CreateRenderer());

		if (renderer->Initialize(hWnd, _width, _height) != true)
			return false;

		static uint32 nowWidth = _width;
		static uint32 nowHeight = _height;

		// �������� ����� ���մϴ�..
		// ������ �Ŵ��� �ҷ�����.
		WindowManager::GetInstance()->SetOnResizeFunc([&](uint32 width, uint32 height)
			{
				renderer->SetClientSize(width, height);
				renderer->OnResize();
				UIManager::GetInstance()->OnResize(width, height);
			}
		);

		WindowManager::GetInstance()->SetOnMouseMoveFunc([&](float x, float y)
			{
				InputManager::GetInstance()->SetMousePos(x, y);
			}
		);

		return true;
	}

	bool GraphicsManager::LateInit()
	{
		renderer->LateInitialize();

		return true;
	}

	void GraphicsManager::Update()
	{
		/// ȭ�� ���ſ� �־ ������ ������ �����ݴϴ�..
		///
	}

	void GraphicsManager::Release()
	{
		if (renderer != nullptr)
			renderer->Finalize();
		renderer.reset();

		if (graphicsDLL != nullptr)
			FreeLibrary(graphicsDLL);
	}

	void GraphicsManager::PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassDirectionalLight(color, direction, power, lightViewProj);
	}

	void GraphicsManager::PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassPointLight(color, position, power, range, lightViewProj);
	}

	void GraphicsManager::PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, float halfAngle, float range, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassSpotLight(color, direction, power, halfAngle, range, lightViewProj);
	}

	void GraphicsManager::CreateUITest(HWND hWnd)
	{
		// �׽�Ʈ�� ��������Ʈ
				// �ش� �κ��� �������� ����� �ش�.
		auto resourceManager = ResourceManager::GetInstance();

		resourceManager->LoadCubeMesh();

		/*uint64 id = resourceManager->Load<Texture>("Smol", L"Resources/UI/Smol.png");

		testSprite->SetTexture(resourceManager->Get<Texture>(id));
		testSprite->SetPosition({200.f, 100.f, 0.1f});
		testSprite->SetWidth(200.f);
		testSprite->SetHeight(200.f);*/
	}

	void GraphicsManager::UIRender(float tick)
	{
		//testCanvas->Render(tick);
	}

	std::shared_ptr<Texture> GraphicsManager::LoadTexture(std::wstring path)
	{
		auto resourceManager = ResourceManager::GetInstance();

		return resourceManager->LoadTexture(path);
	}

	std::shared_ptr<GraphicsManager> GraphicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<GraphicsManager>();

		return instance;
	}
}

#include "GamePch.h"
#include "GameEngine.h"

#include "InputManager.h"
#include "Timer.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "NavigationManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<GameEngine> GameEngine::gameEngineInstance = nullptr;

	void GameEngine::Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight)
	{

		// ��� �Ŵ����� Init �մϴ�.
		WindowManager::GetInstance()->InitWindow(gameTitle, hInst, screenWidth, screenHeight);
		Timer::GetInstance()->Init();
		InputManager::GetInstance();
		GraphicsManager::GetInstance()->Init(WindowManager::GetInstance()->GetHWND(), screenWidth, screenHeight);
		UIManager::GetInstance()->Init();
		SceneManager::GetInstance()->Init();
		NavigationManager::GetInstance();

		// ��� �Ŵ����� Init�� ������.. �����츦 ����ݴϴ�.
		WindowManager::GetInstance()->StartWindow();

		// �ؽ��ĸ� ����� �ҷ�������, �����찡 ������ �ҷ��;��մϴ� (WIC �ؽ����� Ư¡)
		GraphicsManager::GetInstance()->LateInit();
		UIManager::GetInstance()->CreateCanvasAll();
	}

	INT GameEngine::Tick()
	{
		// �� ƽ ��������..
			// ������ �Ŵ��� ������ update�� �����ϴ�
		return WindowManager::GetInstance()->Update();
	}

	void GameEngine::Release()
	{
		Timer::GetInstance()->Release();
		SceneManager::GetInstance()->Release();
		NavigationManager::GetInstance()->Release();
		GraphicsManager::GetInstance()->Release();
		WindowManager::GetInstance()->Release();
	}
}

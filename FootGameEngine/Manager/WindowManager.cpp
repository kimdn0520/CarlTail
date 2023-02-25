#include "GamePch.h"
#include "WindowManager.h"

#include "Component/Camera.h"
#include "InputManager.h"
#include "SceneBase.h"
#include "Timer.h"
#include "GraphicsManager.h"
#include "../../FootGraphicsEngine/Manager/ImGUIManager.h"

#include "SceneManager.h"
#include "UIManager.h"

std::shared_ptr<GameEngineSpace::WindowManager> GameEngineSpace::WindowManager::instance = nullptr;

namespace GameEngineSpace
{
	// ������ Ŭ������ ����ϱ� ���� ��������, ���� �ݹ��Լ� ���
	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return WindowManager::GetInstance()->MsgProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowManager::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (GraphicsEngineSpace::ImGUIManager::GetInstance()->SetWndProcHandler(hWnd, message, wParam, lParam) == true)
			return true;

		// �޽��� ���ν���..
			// �ϴ� �����ͺ���.
		switch (message)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		// ��������..
		case WM_SIZE:
		{
			// ���� ������ ������ �ִ� �������� OnResize�� ���ִ� ������ ®����..
				// �׷��Ƚ� ������ ����ƽ���� ������ ���� ���ΰ�..���ӿ����� ���� �ִ� ������ �� ���ΰ�...
				// ������ �Ŵ��� ���ο��� ���ӿ����� ����Ű�� ������..
			clientHeight = HIWORD(lParam);
			clientWidth = LOWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				isMinimized = true;
				isMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				isMinimized = false;
				isMaximized = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				// �ּ�ȭ �Ǿ� �ִ� ����
				if (isMinimized == true)
				{
					isMinimized = false;
				}
				else if (isMaximized == true)
				{
					isMaximized = false;
				}

			}

			if (wParam != SIZE_MINIMIZED)
			{
				// ���� ũ�� ���ͼ� on Resize
				RECT rect;
				GetClientRect(hWnd, &rect);
				this->SetWndSize(rect);
				this->OnResize();

				// ���� ���� ����ī�޶��� ������ �ٲ��ش�.
				SceneManager::GetInstance()->UpdateMainCameraAspectRatio(clientWidth, clientHeight);
			}
			break;
		}

		// ������â Ȱ��ȭ
		case WM_ACTIVATE:
		{
			if (wParam != false)
				InputManager::GetInstance()->isWindowActivated = true;
			else
				InputManager::GetInstance()->isWindowActivated = false;

			break;
		}

		// Ÿ�̸� ���� ����..
		case WM_ENTERSIZEMOVE:
		{
			isResizing = true;

			Timer::GetInstance()->Stop();

			break;
		}
		case WM_EXITSIZEMOVE:
		{
			isResizing = false;

			Timer::GetInstance()->Start();

			// ��������.
			RECT rect;
			GetClientRect(hWnd, &rect);
			this->SetWndSize(rect);
			this->OnResize();

			break;
		}
		case WM_MOUSEMOVE:
		{
			this->OnMouseMove(lParam);

			break;
		}
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	std::shared_ptr<WindowManager> WindowManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<WindowManager>();

		return instance;
	}

	void WindowManager::InitWindow(tstring windowCaption, HINSTANCE hInst, int screenWidth, int screenHeight)
	{
		clientHeight = screenHeight;
		clientWidth = screenWidth;

		const TCHAR* szAppName = windowCaption.c_str();

		HWND _hWnd;

		// ������ Ŭ���� ���
		WNDCLASS wndClass;

		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = MainWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInst;
		wndClass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = szAppName;

		RegisterClass(&wndClass);

		_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 100, 100,
			screenWidth, screenHeight, NULL, NULL, hInst, NULL);

		if (_hWnd == nullptr)
		{
			assert("Failed to Create Window");

			return;
		}

		// ������ �ڵ� ����
		hWnd = _hWnd;

		printf("Window Init Clear\n");

		//ImGui�ʱ�ȭ
		ImGUIManager::GetInstance()->Init();
		ImGUIManager::GetInstance()->InitImplWin(hWnd);

		printf("IMGUI Window Init Clear\n");
	}

	void WindowManager::StartWindow()
	{
		ShowWindow(hWnd, SW_SHOWNORMAL /* nCmdShow�� ���� ������ �κ�.. */);

		UpdateWindow(hWnd);
	}

	void WindowManager::Release()
	{
	}

	INT WindowManager::Update()
	{
		// �޽��� ����
		MSG _msg;

		while (TRUE)
		{
			if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
			{
				if (_msg.message == WM_QUIT) break;
				TranslateMessage(&_msg);
				DispatchMessage(&_msg);
			}
			else
			{
				Timer::GetInstance()->Tick();
				// ���� �Ŵ��� ������Ʈ... => ���� ���� ������ �Ͼ�� �κ�
				InputManager::GetInstance()->Update();
				//UIManager::GetInstance()->Update();
				GraphicsManager::GetInstance()->Update();
				SceneManager::GetInstance()->Update();
				//SoundManager::GetInstance()->Update();

				InputManager::GetInstance()->LateUpdate();

			}
		}

		return static_cast<int>(_msg.wParam);
	}

	void WindowManager::SetWndSize(uint32 width, uint32 height)
	{
		clientWidth = width;
		clientHeight = height;
	}

	void WindowManager::SetWndSize(RECT rect)
	{
		// ū �� ���� ���� �� ���� �̴ϴ�..
		clientWidth = rect.right - rect.left;
		clientHeight = rect.bottom - rect.top;
	}

	void WindowManager::OnResize()
	{
		if (onResize == nullptr)
			return;

		onResize(GetClientWidth(), GetClientHeight());
	}

	void WindowManager::OnMouseMove(LPARAM lParam)
	{
		if (onMouseMove == nullptr)
			return;

		onMouseMove(static_cast<float>(lParam & 0xffff), static_cast<float>((lParam >> (sizeof(WORD) * 8)) & 0xffff));
	}

	void WindowManager::SetOnResizeFunc(std::function<void(uint32, uint32)> onResizeFunc)
	{
		this->onResize = onResizeFunc;
	}

	void WindowManager::SetOnMouseMoveFunc(std::function<void(float, float)> mouseMoveFunc)
	{
		this->onMouseMove = mouseMoveFunc;
	}
}
#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	/**
	 * \brief Ŭ���̾�Ʈ ������ �����ϴ� ������ ���� �Լ�, �������� ������ Ŭ����
	 *
	 * 2022.07.28(��)
	 */
	class WindowManager final
	{
		static std::shared_ptr<WindowManager> instance;

		// â ������ ������ �ʿ��� ����
		bool isMinimized;
		bool isMaximized;
		bool isResizing;

		// â ũ�� ����
		uint32 clientWidth;
		uint32 clientHeight;

		// ������ �ڵ�
		HWND hWnd;

		// â ���� �Լ�.
		std::function<void(uint32, uint32)> onResize;
		std::function<void(float, float)> onMouseMove;

	public:
		WindowManager() = default;
		~WindowManager() = default;

		WindowManager(const WindowManager & other) = delete;
		WindowManager& operator=(const WindowManager & other) = delete;
		WindowManager(const WindowManager && other) = delete;
		WindowManager& operator=(const WindowManager && other) = delete;

		// �޽��� ���ν��� �Լ�
		LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		// ���� ���� ����
		void Release();

		// �ν��Ͻ� ��
		static std::shared_ptr<WindowManager> GetInstance();

		// ������ �ʱ�ȭ
		void InitWindow(tstring windowCaption, HINSTANCE hInst, int screenWidth, int screenHeight);

		// ������ ȭ�鿡 ǥ��
		void StartWindow();

		// Tick..
		INT Update();

		// ���� ����, ����
		HWND GetHWND() { return hWnd; }
		uint32 GetClientWidth() { return clientWidth; }
		uint32 GetClientHeight() { return clientHeight; }

		void SetWndSize(uint32 width, uint32 height);
		void SetWndSize(RECT rect);
		// OnResize
		void OnResize();
		void OnMouseMove(LPARAM lParam);

		// ���� ����
		void SetOnResizeFunc(std::function<void(uint32, uint32)> onResizeFunc);
		void SetOnMouseMoveFunc(std::function<void(float, float)> mouseMoveFunc);
	};
}


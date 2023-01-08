#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	enum class KeyState
	{
		UP,
		DOWN,
		STAY,
		TOGGLE
	};

	/**
	 * \brief �̱������� ��� ��ǲ�� �����ϴ� ��ǲ �Ŵ���.
	 *
	 * 2022.08.01 Dev.BlackFoot
	 */
	class InputManager final
	{
	private:
		// Key
		std::map<int, bool> prevKeyState;
		std::map<int, bool> currKeyState;

		// Cursor Coordinate
		Vector2 prevCursorXY;
		Vector2 currCursorXY;

		// inputManger ����ƽ ����
		static std::shared_ptr<InputManager> instance;

	public:
		// ������ Ȱ��ȭ ���� Input �Ÿ���
		bool isMouseInWindow;
		bool isWindowActivated;

	public:
		InputManager();
		~InputManager();
		// Key Input
		void Update();
		void LateUpdate();
		virtual bool GetInputState(int key, KeyState state);

		// Mouse Pos
		void SetMousePos(float x, float y);
		const Vector2& GetMousePos();
		Vector2 GetMouseMoveVector();

		static std::shared_ptr<InputManager> GetInstance();
	};
}

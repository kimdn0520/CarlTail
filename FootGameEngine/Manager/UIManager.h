#pragma once

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

class GraphicsEngineSpace::ImGUIManager;

namespace GameEngineSpace
{
	class GraphicsManager;
	class InputManager;
	class UICreator;

	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	/**
	 * \brief UI, �ؽ�Ʈ �� ȭ�鿡 2�������� �����ϴ� UI �鸸�� ������ �ϴ� UI Manager Ŭ����
	 *
	 * 2022.08.08(��) Deb.BlackFoot
	 */

	class UIManager final
	{
	private:
		// UI Manager�� �ν��Ͻ�
		static std::shared_ptr<UIManager> instance;

		// ���������� �׷��Ƚ� �Ŵ��� ĳ��
		std::shared_ptr<GraphicsManager> graphicsInstance;
		std::shared_ptr<InputManager> inputInstance;
		std::shared_ptr<UICreator> creator;

		// ĵ����
			// ���ӿ� �ִ� ��� UI ĵ�������� init�� ���� �� ���� ������.
			// UI�� �����Ǵ� ��� �༮���� ó�� Init �� �� ����� enable�� false�� �������ְ�
			// Ư�� ��Ȳ�� �ʿ��� �� �ش� UI ���̵� �����ͼ� ������ش�.
		std::shared_ptr<Canvas> mainCanvas;
		// IMGui ĳ�̿�
		std::shared_ptr<GraphicsEngineSpace::ImGUIManager> imgui;

		// ȭ�� ������ �ľ��ϰ�. �������� �÷�����..
		uint32 nowWidth;
		uint32 nowHeight;

	public:
		UIManager() = default;
		~UIManager() = default;
		UIManager(const UIManager& other) = delete;
		UIManager(const UIManager&& other) = delete;
		UIManager& operator= (const UIManager& other) = delete;
		UIManager& operator= (const UIManager&& other) = delete;

		static std::shared_ptr<UIManager> GetInstance();

		void Init();

		// WIC �̽��� ���ϱ� ���� Canvas ����
		void CreateCanvasAll();

		// ����
		void Render(float tick);

		// ��ư �Է��� ���� ������Ʈ�� LateUpdate
		void Update();
		void LateUpdate();

		void Release();

		std::shared_ptr<Canvas> CreateCanvas(const std::string& UIIDstr);

		// Ư�� ĵ������ ��� ���� ��
		std::shared_ptr<Canvas> GetCanvas(const std::string& canvasIDStr);
		std::shared_ptr<Canvas> GetMainCanvas() { return mainCanvas; }

		// ĵ���� ��������
		void OnResize(uint32 width, uint32 height);

		void ClearUI();

		// ImGUI Render
		void SelectGUIRender();
	};

}

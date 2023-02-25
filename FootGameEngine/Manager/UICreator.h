#pragma once

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	/**
	 * \brief ���ӿ��� ������ ��� UI�� ������ִ� ������ �ϴ� Ŭ�����Դϴ�.
	 * ���⼭ ������� ĵ�������� ��� UIManager�� mainCanvas�ȿ� �����ϰ� �� ���̹Ƿ�
	 * Ŭ������ ���� ������ �����ص� �����մϴ�.
	 *
	 * ����� ĵ���� ��ü�� UI Manager�� Init���� �����˴ϴ�.
	 * �� Ŭ���������� ������ ĵ���� �ȿ� ������� ��� ��������Ʈ �� �ؽ�Ʈ�� �����մϴ�.
	 *
	 * ������ ���� �ǰ�, UI Manager�� Init�� �� ȣ��˴ϴ�.
	 */
	class UICreator
	{
	private:
		std::shared_ptr<Canvas> mainCanvas;

	public:
		void CreateCanvasAll();

	private:
		// ������ ������ ĵ������
		void CreateTitleCanvas();
		void CreateGameHUD();
		void CreateSettingCanvas();
		void CreateInMenuCanvas();
		void CreateCombinationCanvas();
		void CreateDialogCanvas();
		void CreateTestSceneCanvas();
	};

}

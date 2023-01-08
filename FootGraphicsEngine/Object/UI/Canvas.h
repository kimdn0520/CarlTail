#pragma once


#include <map>
#include "TextUI.h"
#include "SpriteUI.h"
#include "ButtonUI.h"

namespace GraphicsEngineSpace
{
	// UI�� �ö� ĵ���� Ŭ����
		// ��� UIBase���� ĵ���� ���� �ö󰣴�.
	class Canvas : public UIBase
	{
		// ���� UI �������� �����ص� map
		std::map<std::string, std::shared_ptr<TextUI>> textUIMap;
		std::map<std::string, std::shared_ptr<SpriteUI>> spriteUIMap;
		std::map<std::string, std::shared_ptr<ButtonUI>> buttonUIMap;
		std::map<std::string, std::shared_ptr<Canvas>> canvasMap;

		// ������ ���õ� PickedUI
		std::shared_ptr<UIBase> selectedUI;
		bool selectMode;

	public:
		Canvas(float width, float height);
		~Canvas();

		// �ܺο��� ��� �����ϵ��� virtual.
		virtual std::shared_ptr<TextUI> CreateTextUI(const std::string& name);
		virtual std::shared_ptr<TextUI> GetTextUI(const std::string& name);

		virtual std::shared_ptr<SpriteUI> CreateSpriteUI(const std::string& name);
		virtual std::shared_ptr<SpriteUI> GetSpriteUI(const std::string& name);

		virtual std::shared_ptr<ButtonUI> CreateButtonUI(const std::string& name);
		virtual std::shared_ptr<ButtonUI> GetButtonUI(const std::string& name);

		virtual std::shared_ptr<Canvas> CreateCanvasUI(const std::string& name, float width, float height);
		virtual std::shared_ptr<Canvas> GetCanvasUI(const std::string& name);


		// ��ư ���� Ȯ�� �ڵ�
		virtual void CheckCollidedButton(float mouseX, float mouseY, bool isClicked);
		// UISelected Ȯ�� �ڵ�
		virtual std::shared_ptr<UIBase> CheckPickedUI(float mouseX, float mouseY, bool isClicked);

		virtual void SetScaleAllCanvas(SimpleMath::Vector2 scale);
		virtual void SetSelectMode(bool selectMode);
		virtual bool GetSelectMode() { return selectMode; }
		virtual std::shared_ptr<UIBase> GetSelectedUI() { return selectedUI; }

		void Render(float tick) override;
		virtual void Release();

	private:
		// ���� � ��ư���� �浹�� �Ͼ�°�.
		std::shared_ptr<ButtonUI> FindCollidedButton(float mouseX, float mouseY);
		std::shared_ptr<UIBase> FindPickedUI(float mouseX, float mouseY);
	};

}

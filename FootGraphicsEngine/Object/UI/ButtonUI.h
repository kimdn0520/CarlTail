#pragma once

#include <functional>
#include "UIBase.h"
#include "Resources/Texture.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	enum class ButtonState
	{
		DEFAULT,	// �⺻ ����
		DOWN,		// ���� �� ����
		UP,			// �� �� ����
		PRESS,		// ������ �ִ� ����
		HOVER,		// �÷��ΰ� �ִ� ����
		ENTER,		// ���� ó�� ������ ����(hover ����, �� ���� ���;��ϴ� �̺�Ʈ)
		DISABLE		// ��� ���ÿ� ������ ���� �ʴ� ���� (bool ������ ���� �ʰ� ButtonState�� ����)
	};

	// �⺻������ Sprite UI�� ����������, ���� �̺�Ʈ���� functor�� ������ �ִ� ��ư UI
	class ButtonUI : public UIBase
	{
		std::shared_ptr<Texture> defaultTexture;
		SimpleMath::Vector4 defaultMaskColor;
		std::shared_ptr<Texture> hoverTexture;
		SimpleMath::Vector4 hoverMaskColor;
		std::shared_ptr<Texture> pressTexture;
		SimpleMath::Vector4 pressMaskColor;
		ButtonState state;

		// �ƹ� ���� ��ȯ���� �ʰ�, �Ű� ������ �ʿ� ���� �Լ� ������
		std::function<void()> clickEvent;	// ��ư�� ������ ��
		std::function<void()> pressEvent;	// ��ư�� ������ ���� ��
		std::function<void()> enterEvent;	// ��ư�� �������� ��
		std::function<void()> hoverEvent;	// ��ư�� ������ �ʰ� ���� ��� �� ��
		std::function<void()> outEvent;		// ��ư���� ����� �� (hover���� ��ư ���� ������ ���� ��)

	public:
		ButtonUI();
		~ButtonUI() override;

		virtual void SetDefaultTexture(std::shared_ptr<Texture> texture);
		virtual void SetDefaultMaskColor(SimpleMath::Vector4 color) { defaultMaskColor = color; }
		virtual void SetDefaultMaskColor(float r, float g, float b, float a);
		virtual void SetHoverTexture(std::shared_ptr<Texture> texture);
		virtual void SetHoverMaskColor(SimpleMath::Vector4 color) { hoverMaskColor = color; }
		virtual void SetHoverMaskColor(float r, float g, float b, float a);
		virtual void SetPressTexture(std::shared_ptr<Texture> texture);
		virtual void SetPressMaskColor(SimpleMath::Vector4 color) { pressMaskColor = color; }
		virtual void SetPressMaskColor(float r, float g, float b, float a);
		virtual void SetButtonState(ButtonState state);

		virtual void SetButtonEnable(bool enable);

		virtual void SetClickEvent(std::function<void()> clickEvent);
		virtual void SetPressEvent(std::function<void()> pressEvent);
		virtual void SetEnterEvent(std::function<void()> enterEvent);
		virtual void SetHoverEvent(std::function<void()> hoverEvent);
		virtual void SetOutEvent(std::function<void()> outEvent);

		virtual ButtonState GetButtonState();

	protected:
		void Render(std::shared_ptr<IRenderer> renderer, float tick) override;

		friend Canvas;

	};
}
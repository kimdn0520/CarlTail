#pragma once

// �ؽ�Ʈ UI�� ȭ�鿡 ���� ����..

#include <string>
#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	class TextUI : public UIBase
	{
		std::wstring text;
		std::string font;
		// ���İ����� �� color
		SimpleMath::Vector4 color;
		float fontSize;

	public:
		TextUI();
		~TextUI() override;
		virtual void SetText(const std::string& text);
		virtual void SetText(const std::wstring& text);
		virtual void SetColor(const SimpleMath::Vector4& color);
		virtual void SetFont(const std::string& fontName);
		virtual void SetFontSize(float fontSize);

		virtual std::string GetText();
		virtual float GetFontSize() { return fontSize; }

	protected:
		void Render(std::shared_ptr<IRenderer> renderer, float tick) override;

		friend Canvas;
	};
}

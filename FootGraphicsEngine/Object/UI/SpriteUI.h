#pragma once

#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Texture;
	class Canvas;

	// 2D SpriteUI�� ȭ�鿡 ���� ���� Ŭ����
	class SpriteUI : public UIBase
	{
		// ����� �ؽ��� ����..���ҽ� �Ŵ����� ������ �ؽ��ĸ� ����Ѵ�.
		std::shared_ptr<Texture> texture;
		SimpleMath::Vector4 maskColor;

	public:
		SpriteUI();
		~SpriteUI() override;

		virtual void SetTexture(std::shared_ptr<Texture> texture);
		virtual void SetMaskColor(SimpleMath::Vector4 color);
		virtual void SetMaskColor(float r, float g, float b, float a);
		virtual void SetMaskAlpha(float a);
		virtual std::shared_ptr<Texture> GetTexture();

	protected:
		void Render(float tick) override;

		friend Canvas;

	};

}
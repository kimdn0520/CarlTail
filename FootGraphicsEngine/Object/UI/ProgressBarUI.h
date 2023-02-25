#pragma once

#include "UIBase.h"

namespace GraphicsEngineSpace
{

	class Texture;
	class Canvas;

	// ���� ���� ~������ �������ٴ� �ǹ�.
	enum class FillDir
	{
		UP,		// ���� ��
		DOWN,	// �Ʒ��� ��
		LEFT,	// �������� ��
		RIGHT	// ���������� ��
	};

	/// 2D Texture�� Progress Bar�� ȭ�鿡 ���� ���� Ŭ����
	///	�⺻������ BackBar�� �׸��� �� ���� ProgressBar�� �׸���.
	///	ProgressBar�� Bar�� ���� ����, �� % á���� ���� ������ ������ �ִ�.
	class ProgressBarUI : public UIBase
	{
		// ����� �ؽ��� ����
		std::shared_ptr<Texture> backgroundTexture;
		std::shared_ptr<Texture> barTexture;

		// �� �ؽ����� ũ�� ���� (Texture Wipe)�� ���� ����
			// Sprite Batch Draw�� Source�� ����..
			// progressBar �ؽ����� Ư�� �κи��� �巯���ٰ� ���� �����ϴ�..
			// �⺻������ Background�� ��� �׷����� Progress Bar�� texture�� �������ָ� �ȴ�.
		RECT barSrcRect;

		FillDir direction;
		float fillPercent;

	public:
		ProgressBarUI();
		~ProgressBarUI() override;

		virtual void SetBackgroundTexture(std::shared_ptr<Texture> texture);
		virtual void SetBarTexture(std::shared_ptr<Texture> texture);

		virtual float GetFillPercent() { return fillPercent; }
		virtual void SetFillPercent(float percent);
		virtual void SetFillDirection(FillDir direction);

		// BarTexture�� left, top, right, bottom�� ����ϴ�.
		// => �ش� ��ġ�� percent�� ���ؼ� texture�� �ڸ��ϴ�.
		virtual void SetBarTexRect(long left, long top, long width, long height);

	protected:
		void Render(std::shared_ptr<IRenderer> renderer, float tick) override;

		friend Canvas;

	};
}


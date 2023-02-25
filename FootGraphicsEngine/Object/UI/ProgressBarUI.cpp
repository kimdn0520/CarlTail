#include "GraphicsPch.h"
#include "ProgressBarUI.h"

#include "Resources/Texture.h"
#include "Manager/UIUtilsManager.h"

namespace GraphicsEngineSpace
{
	ProgressBarUI::ProgressBarUI()
		: backgroundTexture(nullptr)
		, barTexture(nullptr)
		, barSrcRect(RECT{ 0, 0, 100, 100 })
		, direction(FillDir::RIGHT)
		, fillPercent(1.0f)
	{
	}

	ProgressBarUI::~ProgressBarUI()
	{
	}

	void ProgressBarUI::SetBackgroundTexture(std::shared_ptr<Texture> texture)
	{
		backgroundTexture = texture;
	}

	void ProgressBarUI::SetBarTexture(std::shared_ptr<Texture> texture)
	{
		barTexture = texture;
	}

	void ProgressBarUI::SetFillPercent(float percent)
	{
		if (percent < 0.0f)
			percent = 0.0f;
		else if (percent > 1.0f)
			percent = 1.0f;

		fillPercent = percent;
	}

	void ProgressBarUI::SetFillDirection(FillDir direction)
	{
		this->direction = direction;
	}

	void ProgressBarUI::SetBarTexRect(long left, long top, long width, long height)
	{
		// �ؽ����� ������ġ
		barSrcRect.top = top;
		barSrcRect.left = left;

		// �ʺ�� ����.
		barSrcRect.bottom = top + height;
		barSrcRect.right = left + width;
	}

	void ProgressBarUI::Render(std::shared_ptr<IRenderer> renderer, float tick)
	{
		if (isEnable != true)
			return;

		// �׸� �׸���
		if (barTexture != nullptr)
		{
			// �⺻������ background ���� �׷��ݴϴ�.
			auto screenPos = GetScreenPosition();
			auto screenScale = GetScreenScale();
			auto batch = UIUtilsManager::GetInstance()->GetSpriteBatch();

			// �׸��� �׷��� Rect
			RECT destRect = {
				static_cast<long>(screenPos.x),
			static_cast<long>(screenPos.y),
			static_cast<long>(screenPos.x + (width * screenScale.x)),
			static_cast<long>(screenPos.y + (height * screenScale.y))
			};

			if (backgroundTexture != nullptr)
			{
				batch->Begin(DirectX::SpriteSortMode_Texture, UIUtilsManager::GetInstance()->GetBlendState().Get(), nullptr, UIUtilsManager::GetInstance()->GetDepthState().Get());
				batch->Draw(backgroundTexture->GetMapSRV().Get(), destRect, nullptr, Colors::White,
					0.0f, { 0.f, 0.f }, DirectX::SpriteEffects_None, screenPos.z);
				batch->End();
			}

			// �������� ä������ ��Ȳ�� ���� Render�� �մϴ�.
			RECT barSrc = barSrcRect;

			switch (direction)
			{
			case FillDir::RIGHT:
			{
				// ���������� ���� ���̱� ������ �������� ��ǥ�� �����մϴ�.
				destRect.right = static_cast<long>(destRect.left + width * screenScale.x * fillPercent);
				barSrc.right = static_cast<long>(barSrcRect.right * fillPercent);
				break;
			}

			case FillDir::LEFT:
			{
				destRect.left = static_cast<long>(destRect.right - width * screenScale.x * fillPercent);
				barSrc.left = static_cast<long>(barSrc.right - (barSrcRect.right - barSrcRect.left) * fillPercent);
				break;
			}

			case FillDir::UP:
			{
				destRect.top = static_cast<long>(destRect.bottom - height * screenScale.y * fillPercent);
				barSrc.top = static_cast<long>(barSrc.bottom - (barSrcRect.bottom - barSrcRect.top) * fillPercent);

				break;
			}

			case FillDir::DOWN:
			{
				destRect.bottom = static_cast<long>(destRect.top + height * screenScale.y * fillPercent);
				barSrc.bottom = static_cast<long>(barSrcRect.bottom * fillPercent);

				break;
			}

			default:
			{
				break;
			}
			}

			batch->Begin(DirectX::SpriteSortMode_Texture, UIUtilsManager::GetInstance()->GetBlendState().Get(), nullptr, UIUtilsManager::GetInstance()->GetDepthState().Get());
			batch->Draw(barTexture->GetMapSRV().Get(), destRect, &barSrc, Colors::White,
				0.0f, { 0.0f, 0.0f }, DirectX::SpriteEffects_None, screenPos.z);
			batch->End();


			for (auto iter : children)
			{
				if (iter != nullptr)
					iter->Render(renderer, tick);
			}
		}
	}
}

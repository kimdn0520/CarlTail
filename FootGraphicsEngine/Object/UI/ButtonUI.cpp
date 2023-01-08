#include "GraphicsPch.h"
#include "ButtonUI.h"

#include "Manager/UIUtilsManager.h"

namespace GraphicsEngineSpace
{
	ButtonUI::ButtonUI()
		: defaultTexture(nullptr)
		, defaultMaskColor(Colors::White)
		, hoverTexture(nullptr)
		, hoverMaskColor(Colors::LightGray)
		, pressTexture(nullptr)
		, pressMaskColor(Colors::DarkGray)
		, state(ButtonState::DEFAULT)
		, clickEvent(nullptr)
		, pressEvent(nullptr)
		, enterEvent(nullptr)
		, hoverEvent(nullptr)
		, outEvent(nullptr)
	{
	}

	ButtonUI::~ButtonUI()
	{
	}

	void ButtonUI::SetDefaultTexture(std::shared_ptr<Texture> texture)
	{
		this->defaultTexture = texture;
	}

	void ButtonUI::SetDefaultMaskColor(float r, float g, float b, float a)
	{
		defaultMaskColor = SimpleMath::Vector4{r, g, b, a};
	}

	void ButtonUI::SetHoverTexture(std::shared_ptr<Texture> texture)
	{
		this->hoverTexture = texture;
	}

	void ButtonUI::SetHoverMaskColor(float r, float g, float b, float a)
	{
		defaultMaskColor = SimpleMath::Vector4{ r, g, b, a };
	}

	void ButtonUI::SetPressTexture(std::shared_ptr<Texture> texture)
	{
		this->pressTexture = texture;
	}

	void ButtonUI::SetPressMaskColor(float r, float g, float b, float a)
	{
		defaultMaskColor = SimpleMath::Vector4{ r, g, b, a };
	}

	// �� ƽ ���鼭 ButtonState�� �������ݴϴ�.
		// �� �̺�Ʈ�� �����ϸ� �����ŵ�ϴ�.
	void ButtonUI::SetButtonState(ButtonState state)
	{
		// ���� ������Ʈ�� hover�鼭 ���� State�� hover�� �ƴϸ�
			// ��ư ������ ����
		if (this->state == ButtonState::HOVER && state != ButtonState::HOVER)
		{
			// out Event ����
			if (outEvent != nullptr)
				outEvent();
		}

		switch (state)
		{
			case ButtonState::ENTER:
			{
				if (enterEvent != nullptr)
					enterEvent();

				break;
			}

			case ButtonState::PRESS:
			{
				if (pressEvent != nullptr)
					pressEvent();

				break;
			}

			// ��ư�� ������ ���� ���� => click Event ����
			case ButtonState::UP:
			{
				if (clickEvent != nullptr)
					clickEvent();

				break;
			}

			case ButtonState::HOVER:
			{
				if (hoverEvent != nullptr)
					hoverEvent();

				break;
			}

			case ButtonState::DEFAULT:
			case ButtonState::DOWN:
			default:
			break;
		}

		// Button State ����
		this->state = state;
	}

	void ButtonUI::SetClickEvent(std::function<void()> clickEvent)
	{
		this->clickEvent = clickEvent;
	}

	void ButtonUI::SetPressEvent(std::function<void()> pressEvent)
	{
		this->pressEvent = pressEvent;
	}

	void ButtonUI::SetEnterEvent(std::function<void()> enterEvent)
	{
		this->enterEvent = enterEvent;
	}

	void ButtonUI::SetHoverEvent(std::function<void()> hoverEvent)
	{
		this->hoverEvent = hoverEvent;
	}

	void ButtonUI::SetOutEvent(std::function<void()> outEvent)
	{
		this->outEvent = outEvent;
	}

	ButtonState ButtonUI::GetButtonState()
	{
		return state;
	}

	void ButtonUI::Render(float tick)
	{
		// ������ ��ư ���¿� ���� �޶�����..
		if (isEnable != true)
			return;

		// �׸� �׸���.
			// �� �ؽ����� ���ο� ����..
		auto defaultTex = defaultTexture;
		auto hoverTex = hoverTexture;
		auto pressTex = pressTexture;

		// �����̴��� DefaultTexture�� �ݵ�� �����ؾ���..
		if (defaultTex != nullptr)
		{
			auto screenPos = GetScreenPosition();
			auto screenScale = GetScreenScale();
			RECT buttonSize = { static_cast<long>(screenPos.x), static_cast<long>(screenPos.y),
			static_cast<long>(screenPos.x + (width * screenScale.x)), static_cast<long>(screenPos.y + (height * screenScale.y)) };
			auto batch = UIUtilsManager::GetInstance()->GetSpriteBatch();

			// ��Ȳ�� ���� �׸���..
			batch->Begin(DirectX::SpriteSortMode_Texture, UIUtilsManager::GetInstance()->GetBlendState().Get(), nullptr, UIUtilsManager::GetInstance()->GetDepthState().Get());

			// ������ Draw �Լ��� ȣ���ϴ� Switch ��
			switch (state)
			{
				// ������ ���� ��.
				case ButtonState::DOWN:
				case ButtonState::PRESS:
				{

					if (pressTex != nullptr)
						batch->Draw(pressTex->GetMapSRV().Get(), buttonSize, nullptr,
							DirectX::Colors::White, 0.0f, { 0.f, 0.f }, DirectX::SpriteEffects_None, screenPos.z);

					// �ؽ��İ� ������.. color ����ũ�� �ٲ㺸��..
					batch->Draw(defaultTex->GetMapSRV().Get(), buttonSize, nullptr,
						pressMaskColor, 0.0f, { 0.f, 0.f }, DirectX::SpriteEffects_None, screenPos.z);

					break;
				}

				// hover�� �ش��ϴ� State
				case ButtonState::UP:
				case ButtonState::HOVER:
				case ButtonState::ENTER:
				{
					if (hoverTex != nullptr)
						batch->Draw(hoverTex->GetMapSRV().Get(), buttonSize, nullptr,
							DirectX::Colors::White, 0.0f, { 0.f, 0.f }, DirectX::SpriteEffects_None, screenPos.z);

					// �ؽ��İ� ������.. color ����ũ�� �ٲ㺸��..
					batch->Draw(defaultTex->GetMapSRV().Get(), buttonSize, nullptr,
						hoverMaskColor, 0.0f, { 0.f, 0.f }, DirectX::SpriteEffects_None, screenPos.z);

					break;
				}

				// ������ ���.. �׳� �׷��ݴϴ�.
				case ButtonState::DEFAULT:
				default:
				{
					batch->Draw(defaultTex->GetMapSRV().Get(), buttonSize, nullptr,
						defaultMaskColor, 0.0f, { 0.f, 0.f }, DirectX::SpriteEffects_None, screenPos.z);

					break;
				}
			}

			batch->End();
		}

		for (auto iter : children)
		{
			if (iter != nullptr)
				iter->Render(tick);
		}
	}
}

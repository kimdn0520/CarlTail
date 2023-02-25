#include "GraphicsPch.h"

#include "Canvas.h"
#include "TextUI.h"

#include "Manager/ResourceManager.h"
#include "Manager/UIUtilsManager.h"

namespace GraphicsEngineSpace
{
	Canvas::Canvas(float width, float height)
		: selectedUI(nullptr)
		, selectMode(false)
	{
		this->width = width;
		this->height = height;
	}

	Canvas::~Canvas()
	{
		Release();
	}

	std::shared_ptr<TextUI> Canvas::CreateTextUI(const std::string& name)
	{
		auto textUI = textUIMap.find(name);
		if (textUI != textUIMap.end())
			return textUI->second;

		std::string newUIName = name;
		std::shared_ptr<TextUI> newTextUI = std::make_shared<TextUI>();

		newTextUI->SetName(name);
		newTextUI->SetParent(shared_from_this());

		textUIMap.insert(std::pair{ name, newTextUI });

		return textUIMap.at(name);
	}

	std::shared_ptr<TextUI> Canvas::GetTextUI(const std::string& name)
	{
		auto textUI = textUIMap.find(name);
		if (textUI == textUIMap.end())
			return nullptr;

		return textUI->second;
	}

	std::shared_ptr<SpriteUI> Canvas::CreateSpriteUI(const std::string& name)
	{
		auto spriteUI = spriteUIMap.find(name);
		if (spriteUI != spriteUIMap.end())
			return spriteUI->second;

		std::string newUIName = name;
		std::shared_ptr<SpriteUI> newSpriteUI = std::make_shared<SpriteUI>();

		newSpriteUI->SetName(name);
		newSpriteUI->SetParent(shared_from_this());

		spriteUIMap.insert(std::pair{ name, newSpriteUI });

		return spriteUIMap.at(name);
	}

	std::shared_ptr<SpriteUI> Canvas::GetSpriteUI(const std::string& name)
	{
		auto spriteUI = spriteUIMap.find(name);
		if (spriteUI == spriteUIMap.end())
			return nullptr;

		return spriteUI->second;
	}

	std::shared_ptr<ButtonUI> Canvas::CreateButtonUI(const std::string& name)
	{
		auto buttonUI = buttonUIMap.find(name);
		if (buttonUI != buttonUIMap.end())
			return buttonUI->second;

		std::string newUIName = name;
		std::shared_ptr<ButtonUI> newButtonUI = std::make_shared<ButtonUI>();

		newButtonUI->SetName(name);
		newButtonUI->SetParent(shared_from_this());

		buttonUIMap.insert(std::pair{ name, newButtonUI });

		return buttonUIMap.at(name);
	}

	std::shared_ptr<ButtonUI> Canvas::GetButtonUI(const std::string& name)
	{
		auto buttonUI = buttonUIMap.find(name);
		if (buttonUI == buttonUIMap.end())
			return nullptr;

		return buttonUI->second;
	}

	std::shared_ptr<Canvas> Canvas::CreateCanvasUI(const std::string& name, float width, float height)
	{
		auto canvasUI = canvasMap.find(name);
		if (canvasUI != canvasMap.end())
			return canvasUI->second;

		std::string newUIName = name;
		std::shared_ptr<Canvas> newCanvasUI = std::make_shared<Canvas>(width, height);

		newCanvasUI->SetName(name);
		newCanvasUI->SetParent(shared_from_this());

		canvasMap.insert(std::pair{ name, newCanvasUI });

		return canvasMap.at(name);
	}

	std::shared_ptr<Canvas> Canvas::GetCanvasUI(const std::string& name)
	{
		auto canvasUI = canvasMap.find(name);
		if (canvasUI == canvasMap.end())
			return nullptr;

		return canvasUI->second;
	}

	std::shared_ptr<ProgressBarUI> Canvas::CreateProgressBarUI(const std::string& name)
	{
		auto progressBar = progressBarUIMap.find(name);
		if (progressBar != progressBarUIMap.end())
			return progressBar->second;

		std::string newUIName = name;
		std::shared_ptr<ProgressBarUI> newProgressBar = std::make_shared<ProgressBarUI>();

		newProgressBar->SetName(name);
		newProgressBar->SetParent(shared_from_this());

		progressBarUIMap.insert(std::pair{ name, newProgressBar });

		return progressBarUIMap.at(name);
	}

	std::shared_ptr<ProgressBarUI> Canvas::GetProgressBarUI(const std::string& name)
	{
		auto progressBar = progressBarUIMap.find(name);
		if (progressBar == progressBarUIMap.end())
			return nullptr;

		return progressBar->second;
	}

	void Canvas::CheckCollidedButton(float mouseX, float mouseY, bool isClicked)
	{
		// ��� button�� State�� �������ݴϴ�.
		std::shared_ptr<ButtonUI> collidedButton = FindCollidedButton(mouseX, mouseY);

		if (collidedButton == nullptr)
			return;

		if (isClicked == true)
		{
			// �浹�� ��ư�� ���¿� ����..
			if (collidedButton->state == ButtonState::DOWN ||
				collidedButton->state == ButtonState::PRESS)
				collidedButton->SetButtonState(ButtonState::PRESS);
			else
				collidedButton->SetButtonState(ButtonState::DOWN);
		}
		else
		{
			if (collidedButton->state == ButtonState::DOWN ||
				collidedButton->state == ButtonState::PRESS)
				collidedButton->SetButtonState(ButtonState::UP);
			else if (collidedButton->state == ButtonState::DEFAULT)
				collidedButton->SetButtonState(ButtonState::ENTER);
			else
				collidedButton->SetButtonState(ButtonState::HOVER);
		}
	}

	std::shared_ptr<UIBase> Canvas::CheckPickedUI(float mouseX, float mouseY, bool isClicked)
	{
		std::shared_ptr<UIBase> pickedUI = FindPickedUI(mouseX, mouseY);

		if (pickedUI == nullptr)
		{
			return selectedUI;
		}

		if (isClicked == true)
		{
			// ������ ���õ� ���� �־��ٸ�..
			if(selectedUI != nullptr)
				selectedUI->SetSelected(UIState::DEFAULT);

			if (pickedUI->GetSelected() == UIState::PICK)
				pickedUI->SetSelected(UIState::DEFAULT);
			else
			{
				pickedUI->SetSelected(UIState::PICK);
				selectedUI = pickedUI;
			}
		}
		else
		{
			// ������ ���õ� ���θ� ���������� �ʴ´�.
			if (pickedUI->GetSelected() != UIState::PICK)
				pickedUI->SetSelected(UIState::HOVER);
		}

		return selectedUI;
	}

	void Canvas::SetScaleAllCanvas(SimpleMath::Vector2 scale)
	{
		for (auto& canvas : canvasMap)
		{
			if (canvas.second != nullptr)
				canvas.second->SetScale(scale);
		}
	}

	void Canvas::SetSelectMode(bool selectMode)
	{
		// �ڽĵ��� selectMode�� ����
		for (auto& canvas : canvasMap)
		{
			canvas.second->SetSelectMode(selectMode);
		}

		this->selectMode = selectMode;
	}

	void Canvas::Render(std::shared_ptr<IRenderer> renderer, float tick)
	{
		if (isEnable != true)
			return;

		auto batch = UIUtilsManager::GetInstance()->GetSpriteBatch();
		auto limTexture = UIUtilsManager::GetInstance()->GetLimTexture();

		for (auto iter : children)
		{
			if (iter != nullptr)
			{
				renderer->GraphicsDebugBeginEvent(iter->GetName());
				iter->Render(renderer, tick);
				renderer->GraphicsDebugEndEvent();
			}

			if (selectMode == true)
			{
				if (iter->GetSelected() == UIState::DEFAULT)
					continue;

				// ĵ������ ���� ������ ����..
				if (std::dynamic_pointer_cast<Canvas>(iter) != nullptr)
					continue;

				SimpleMath::Vector3 UIScreenPos = iter->GetScreenPosition();
				SimpleMath::Vector2 UIScreenScale = iter->GetScreenScale();
				RECT UIRect = { static_cast<long>(UIScreenPos.x), static_cast<long>(UIScreenPos.y),
					static_cast<long>(UIScreenPos.x + (iter->GetWidth() * UIScreenScale.x)),
					static_cast<long>(UIScreenPos.y + (iter->GetHeight() * UIScreenScale.y)) };

				// ��Ȳ�� ���� �׸���
				batch->Begin(DirectX::SpriteSortMode_Texture, UIUtilsManager::GetInstance()->GetBlendState().Get(), nullptr, UIUtilsManager::GetInstance()->GetDepthState().Get());

				switch (iter->GetSelected())
				{
					case UIState::HOVER:
					{
						batch->Draw(limTexture->GetMapSRV().Get(), UIRect, nullptr,
							DirectX::Colors::LimeGreen, 0.0f, { 0.0f, 0.0f }, DirectX::SpriteEffects_None, UIScreenPos.z);
						break;
					}

					case UIState::PICK:
					{
						batch->Draw(limTexture->GetMapSRV().Get(), UIRect, nullptr,
							DirectX::Colors::Red, 0.0f, { 0.0f, 0.0f }, DirectX::SpriteEffects_None, UIScreenPos.z);
						break;
					}

					case UIState::DEFAULT:
					default:
					break;
				}

				batch->End();
			}
		}

	}

	void Canvas::Release()
	{
		for (auto text : textUIMap)
		{
			SafeReset(text.second)
		}

		for (auto sprite : spriteUIMap)
		{
			SafeReset(sprite.second)
		}

		for (auto button : buttonUIMap)
		{
			SafeReset(button.second)
		}

		textUIMap.clear();
		spriteUIMap.clear();
		buttonUIMap.clear();
	}

	std::shared_ptr<ButtonUI> Canvas::FindCollidedButton(float mouseX, float mouseY)
	{
		// �� ȭ��(ĵ����)�� �浹�� ��ư�� �ϳ����� �Ѵ� => ��� ������ �������� �����Ѵ�.
		std::shared_ptr<ButtonUI> collidedButton = nullptr;
		// ���� ��
		float minZ = 1.0f;

		for (auto& canvas : canvasMap)
		{
			if (canvas.second == nullptr || canvas.second->isEnable != true)
				continue;

			std::shared_ptr<ButtonUI> tmpButton = canvas.second->FindCollidedButton(mouseX, mouseY);

			if (tmpButton == nullptr)
				continue;

			if (tmpButton->GetScreenPosition().z <= minZ)
			{
				if (collidedButton != nullptr)
					collidedButton->SetButtonState(ButtonState::DEFAULT);

				collidedButton = tmpButton;
				minZ = tmpButton->GetScreenPosition().z;
			}
		}

		for (auto& button : buttonUIMap)
		{
			// ��ư�� ���ٸ�..
			if (button.second == nullptr)
				continue;

			// ��� �Ұ��� �ϰ� �����Ǿ� ������
			if (button.second->isEnable != true)
			{
				button.second->SetButtonState(ButtonState::DEFAULT);
				continue;
			}

			// ��ư�� ��� �Ұ����� ���¸�.. üũ�� ���� �ʽ��ϴ�.
			if (button.second->state == ButtonState::DISABLE)
				continue;

			// AABB Check
			auto buttonScreenPos = button.second->GetScreenPosition();
			auto buttonScreenScale = button.second->GetScreenScale();

			// ���콺�� ��ġ�� ��ư���� ��
			if (mouseX < buttonScreenPos.x || mouseY < buttonScreenPos.y)
			{
				button.second->SetButtonState(ButtonState::DEFAULT);
				continue;
			}

			// ���콺�� ��ġ�� ��ư���� ��
			if (mouseX > buttonScreenPos.x + button.second->GetWidth() * buttonScreenScale.x ||
				mouseY > buttonScreenPos.y + button.second->GetHeight() * buttonScreenScale.y)
			{
				button.second->SetButtonState(ButtonState::DEFAULT);
				continue;
			}

			// z�� �Ǻ�
			if (buttonScreenPos.z <= minZ)
			{
				if (collidedButton != nullptr)
					collidedButton->SetButtonState(ButtonState::DEFAULT);

				collidedButton = button.second;
				minZ = buttonScreenPos.z;
			}
		}

		if (collidedButton == nullptr)
			return nullptr;

		return collidedButton;
	}

	std::shared_ptr<UIBase> Canvas::FindPickedUI(float mouseX, float mouseY)
	{
		// �� ȭ��(ĵ����)�� pick�� UI ��ü�� �ϳ���.. ��� UI ��ü�� ����..
		std::shared_ptr<UIBase> pickedUI = nullptr;
		float minZ = 1.0f;

		SimpleMath::Vector3 screenPos;
		SimpleMath::Vector2 screenScale;

		for (auto& canvas : canvasMap)
		{
			if (canvas.second == nullptr || canvas.second->isEnable != true)
				continue;

			std::shared_ptr<UIBase> tmpUI = canvas.second->FindPickedUI(mouseX, mouseY);

			if (tmpUI == nullptr)
				continue;

			if (tmpUI->GetScreenPosition().z <= minZ)
			{
				if (pickedUI != nullptr)
					pickedUI->SetSelected(UIState::DEFAULT);

				pickedUI = tmpUI;
				minZ = tmpUI->GetScreenPosition().z;
			}
		}

		for (auto& childUI : children)
		{
			if (childUI == nullptr)
				continue;

			// child�� ĵ������ ������ ��ŵ
			if (std::dynamic_pointer_cast<Canvas>(childUI) != nullptr)
				continue;

			// ���� ������ �Ʒ��� ���� ���� �ʴ´�.
			if (childUI->GetSelected() == UIState::PICK)
			{
				continue;
			}

			if (childUI->GetEnable() != true)
			{
				childUI->SetSelected(UIState::DEFAULT);
				continue;
			}

			screenPos = childUI->GetScreenPosition();
			screenScale = childUI->GetScreenScale();

			if (mouseX < screenPos.x || mouseY < screenPos.y)
			{
				childUI->SetSelected(UIState::DEFAULT);
				continue;
			}

			// ���콺�� ��ġ�� UI���� ��
			if (mouseX > screenPos.x + childUI->GetWidth() * screenScale.x ||
				mouseY > screenPos.y + childUI->GetHeight() * screenScale.y)
			{
				childUI->SetSelected(UIState::DEFAULT);
				continue;
			}

			// ���̰� �Ǻ�
			if (screenPos.z <= minZ)
			{
				if (pickedUI != nullptr)
					pickedUI->SetSelected(UIState::DEFAULT);

				pickedUI = childUI;
				minZ = screenPos.z;
			}
		}

		if (pickedUI == nullptr)
			return nullptr;

		return pickedUI;
	}
}

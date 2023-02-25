#pragma once

// ��� UI���� ������ �־���ϴ� �⺻���� ���� �� �Լ��� ������ �ִ� Ŭ����
	// ����� Render�� �ְ� ���Ŀ� pivot, Anchor ���� ���� �� �ִ�.

#include <vector>
#include "Interface/IRenderer.h"

namespace GraphicsEngineSpace
{
	class Texture;

	// Pick ���θ��� �˱� ���� Enum Class
	enum class UIState
	{
		DEFAULT,
		HOVER,
		PICK
	};

	class UIBase : public std::enable_shared_from_this<UIBase>
	{
	protected:
		float width;
		float height;

		bool isEnable;
		// Pick�� ���ؼ� �����ص� �� ����.
		UIState isSelected;

		std::weak_ptr<UIBase> parent;
		std::vector<std::shared_ptr<UIBase>> children;

		SimpleMath::Vector3 position;
		SimpleMath::Vector3 rotation;
		SimpleMath::Vector2 scale;

		std::string name;

	private:
		// ��ũ�������� ��ġ
		SimpleMath::Vector3 screenPosition;

	protected:
		// ���� ������
		UIBase()
			: width(0.0f)
			, height(0.0f)
			, isEnable(true)
			, isSelected(UIState::DEFAULT)
			, parent(std::weak_ptr<UIBase>())
			, position(SimpleMath::Vector3::Zero)
			, rotation(SimpleMath::Vector3::Zero)
			, scale(SimpleMath::Vector2{ 1.0f, 1.0f })
			, screenPosition(SimpleMath::Vector3::Zero)
		{}

	public:
		virtual ~UIBase() = default;

		virtual void Render(std::shared_ptr<IRenderer> renderer, float tick = 0.0f) abstract;

		// ���� ����
		void SetName(const std::string& name) { this->name = name; }
		void SetPosition(const SimpleMath::Vector3& position) { this->position = position; }
		void SetRotation(const SimpleMath::Vector3& rotation) { this->rotation = rotation; }
		void SetScale(const SimpleMath::Vector2& scale) { this->scale = scale; }
		void SetWidth(float width) { this->width = width; }
		void SetHeight(float height) { this->height = height; }
		void SetEnable(bool enable) { this->isEnable = enable; }
		void SetSelected(UIState selected) { this->isSelected = selected; }
		void SetParent(std::shared_ptr<UIBase> parent)
		{
			// null üũ
			if (parent == nullptr)
				return;

			// ���� parent ���� Ȯ��
			if (this->parent.lock() != nullptr)
				this->parent.lock()->PopChildren(shared_from_this());

			this->parent = parent;
			parent->SetChildren(shared_from_this());
		}

		// ���� ����
		const std::string& GetName() { return name; }
		const SimpleMath::Vector3& GetPosition() { return position; }
		const SimpleMath::Vector3& GetRotation() { return rotation; }
		const SimpleMath::Vector2& GetScale() { return scale; }
		float GetWidth() { return width; }
		float GetHeight() { return height; }
		bool GetEnable() { return isEnable; }
		UIState GetSelected() { return isSelected; }
		RECT GetUIRect()
		{
			auto screenPosition = GetScreenPosition();

			return RECT
			{
				static_cast<long>(screenPosition.x),
				static_cast<long>(screenPosition.y),
				static_cast<long>(screenPosition.x + width),
				static_cast<long>(screenPosition.y + height)
			};
		}

		SimpleMath::Vector2 GetScreenScale()
		{
			SimpleMath::Vector2 result = scale;

			if (parent.lock() != nullptr)
			{
				const SimpleMath::Vector2& parentScreenScale = parent.lock()->GetScreenScale();

				result.x *= parentScreenScale.x;
				result.y *= parentScreenScale.y;
			}

			return result;
		}

		SimpleMath::Vector3 GetScreenPosition()
		{
			auto screenPos = SimpleMath::Vector3::Zero;
			SimpleMath::Vector3 screenScale = {GetScreenScale().x, GetScreenScale().y, 1.0f};
			
			if (parent.lock() != nullptr)
			{
				// �θ� ������..
					// ��� �� �κ��� ���� ������ ���� �Ǵ��ϸ� �Ǻ��� ��Ŀ�� �߿��� ��..
				// �θ��� �Ǻ�, ��Ŀ�� ���� ��ũ���� �δ� ���� �޶�����.
				// �⺻������ Center Center�� �����غ���.
				screenPos = parent.lock()->GetScreenPosition();
				auto parentScreenScale = SimpleMath::Vector3{ parent.lock()->GetScreenScale().x, parent.lock()->GetScreenScale().y, 1.0f };

				screenPos.x += parent.lock()->width / 2 * parentScreenScale.x;
				screenPos.y += parent.lock()->height / 2 * parentScreenScale.y;
			}

			screenPos.x -= width / 2 * screenScale.x;
			screenPos.y -= height / 2 * screenScale.y;

			return position * screenScale + screenPos;
		}

	private:
		void SetChildren(std::shared_ptr<UIBase> child)
		{
			// ���� child�� �������.
			for (int i = 0; i < this->children.size(); i++)
			{
				if (this->children[i] == child)
					return;
			}

			this->children.push_back(child);
		}

		void PopChildren(std::shared_ptr<UIBase> child)
		{
			for (int i = 0; i < this->children.size(); i++)
			{
				if (this->children[i] == child)
				{
					this->children.erase(this->children.begin() + i);

					return;
				}
			}
		}
	};

}
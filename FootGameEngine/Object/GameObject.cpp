#include "GamePch.h"
#include "GameObject.h"
#include "ComponentBase.h"
#include "ITriggerable.h"


namespace GameEngineSpace
{

	GameObject::GameObject() :
		name(TEXT("GameObject"))
		, tag("Default")
		, parent(std::weak_ptr<GameObject>())
		, children(std::vector<std::shared_ptr<GameObject>>())
		, components(std::vector<std::shared_ptr<ComponentBase>>())
		, layer(0)
	{
		// ���� �ÿ��� ���� ��ü�� ���õ� ����Ʈ �����Ͱ� �������� �ʾұ� ������.. �ϴ��� ��������� ������ش�.
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Release()
	{
		if (parent.expired() != true)
		{
			parent.lock().reset();
		}

		transform->Release();

		// �θ� ��ü�� Release�Ǹ�.. �ڽ� ��ü�鵵 ��� Release �ȴ�.
		for (auto& child : children)
		{
			child->Release();
		}

		// ������Ʈ�� �������.
		for (auto& component : components)
		{
			component->Release();
		}
	}

	// �Ʒ��� �������̵� �� �Լ����� ��� ������Ʈ���� �ش� �Լ��� �ҷ��ش�.
	void GameObject::Awake()
	{
		for (auto& component : components)
		{
			component->Awake();
		}
	}

	void GameObject::Start()
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->Start();
				// ��ŸƮ�� ���� ������..
				component->SetIsStarted(true);
			}
		}
	}

	void GameObject::FixedUpdate(float tick)
	{
		for (auto& component : components)
		{
			component->FixedUpdate(tick);
		}
	}

	void GameObject::PreUpdate(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->PreUpdate(tick);
			}
		}
	}

	void GameObject::Update(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->Update(tick);
			}
		}
	}

	void GameObject::LateUpdate(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->LateUpdate(tick);
			}
		}
	}

	void GameObject::DebugIMGUIRender(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->DebugIMGUIRender(tick);
			}
		}
	}

	std::shared_ptr<GameObject> GameObject::GetChild(tstring _name)
	{
		// �̸��� �ش��ϴ� �ڽ��� ã�Ƽ� ��ȯ
		for (const auto& child : children)
		{
			if (child->GetName() == _name)
			{
				return child;
			}
		}

		// ���ϵ带 ��ȸ������ ������.. null ����
		return nullptr;
	}

	void GameObject::SetChild(std::shared_ptr<GameObject> _child)
	{
		// �ڽ� ��Ͽ� �̹� ������ ���� => ����ó��
		for (auto& child : children)
		{
			if (child == _child)
			{
				return;
			}
		}

		// ���ٸ�.. �ڿ� �־��ݴϴ�.
		children.push_back(_child);

		// �׸��� �θ� �������ݴϴ�.
			// shared_from_this == this ������
		_child->SetParent(shared_from_this());
	}

	void GameObject::SetGameObjectEnable(bool _isEnable)
	{
		// ������Ʈ ���鼭 �� is Enable
		this->SetIsEnable(_isEnable);

		for (auto& component : components)
		{
			component->SetIsEnable(_isEnable);

			if (_isEnable == false)
				continue;

			// is Enable�� true�鼭, Start�� ���� �ʾҴٸ�.. Start�� ���ش�.
			if (component->GetIsStarted() != true)
			{
				component->Start();
				component->SetIsStarted(true);
			}
		}

		// �ڽ��� ������..
		for (auto& child : children)
		{
			child->SetGameObjectEnable(_isEnable);
		}

	}

	void GameObject::EraseParent()
	{
		// �θ� �����ݴϴ�..

		// ���� �θ�  null�̸�? ����
		if (parent.lock() == nullptr)
		{
			return;
		}

		// �θ��� �ڽĵ� ��Ͽ��� ���� �����ݴϴ�..
		// �θ��� �ڽĵ� ����� �ް�
		auto& parentsChildren = parent.lock()->GetChildren();

		// �� �ڽ��� �޾Ƽ�
		std::shared_ptr<GameObject> _sharedThis = shared_from_this();

		// �θ��� �ڽ� ��� ���͸� ���鼭 ���� �����ݴϴ�.
		for (int i = 0; i < parentsChildren.size(); i++)
		{
			if (_sharedThis == parentsChildren[i])
			{
				// ���������κ��� i��°�� �ִ� ���� �����ݴϴ�.
				parentsChildren.erase(parentsChildren.begin() + i);

				break;
			}
		}

		// �θ� �����͸� null�� ����ϴ�
		parent = std::weak_ptr<GameObject>();
	}

	bool GameObject::CompareTag(const std::string& tagName)
	{
		return tag == tagName;
	}

	void GameObject::SetParent(std::shared_ptr<GameObject> _parent)
	{
		// ���� �θ�ü�� �־��ٸ�. �����ְ�. ���� �־��ݴϴ�.
		this->EraseParent();

		parent = _parent;
	}

	void GameObject::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		// Ʈ���� �̺�Ʈ ������ ������Ʈ ���� Ʈ���� �̺�Ʈ�� ȣ�����ش�.
		for (auto& triggerCom : triggerables)
			triggerCom->OnTriggerEnter(other);

	}

	void GameObject::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{
		for (auto& triggerCom : triggerables)
			triggerCom->OnTriggerStay(other);
	}

	void GameObject::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
		for (auto& triggerCom : triggerables)
			triggerCom->OnTriggerExit(other);
	}
}
#pragma once

#include "ComponentBase.h"
#include "ITriggerable.h"

namespace GameEngineSpace
{
	/**
	 * \brief ������Ʈ ȭ �� �� �����鼭, Ʈ���� �̺�Ʈ�� �޾Ƽ� Ʈ���� �̺�Ʈ�� ���� ��ų �� �ִ�
	 * MonoBehavior Ŭ����
	 *
	 * ����� Ʈ���� �̺�Ʈ �ۿ� ������, ���Ŀ� �ٸ� �̺�Ʈ���� �߰� �����ϴ�.
	 *
	 * 2022.08.03(��) Dev.BlackFoot
	 */
	class MonoBehaviour : public ComponentBase, public ITriggerable
	{
	public:
		MonoBehaviour(std::weak_ptr<GameObject> gameObj)
			: ComponentBase(gameObj)
		{			
		}

		virtual ~MonoBehaviour()
		{			
		}
	};

}
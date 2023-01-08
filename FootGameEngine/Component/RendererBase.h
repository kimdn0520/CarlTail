#pragma once
#include "Object/ComponentBase.h"
//#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GraphicsEngineSpace
{
	class IDXObject;
}

namespace GameEngineSpace
{
	class GameObject;
	class Transform;

	/**
	 * \brief ������ ������ ���� ������Ʈ�鿡 �ٴ� ������ ������Ʈ���� �⺻�� �Ǵ� ������ ���̽� ������Ʈ �Դϴ�.
	 *
	 * Foot ���������� ������ ������ ������ ������ �� �� �����ϴ�..
	 *
	 * 2022.07.26(ȭ) Dev.BlackFoot
	 */
	class RendererBase : public ComponentBase, public std::enable_shared_from_this<RendererBase>
	{
	protected:
		// �׷��Ƚ� ���������� ������ ����
			// => � ���̴��� ����� ������, ���ؽ� ����, �ε��� ���� => ASE �Ľ��� ������ �� ��ü..?
			// Ȥ�� ASE ���� path. �׷��Ƚ� ���ο��� ASE�� �Ľ��ؼ� ������ �ִ� ������..
		// ���⼭ ������Ʈ�� ������ �ִٸ�?
		std::shared_ptr<GraphicsEngineSpace::IDXObject> renderObj;

		// Ʈ������
		std::shared_ptr<Transform> transform;

	public:
		RendererBase(std::weak_ptr<GameObject> _object);

		virtual ~RendererBase();

		void Release() override;
		
		void Awake() override;

		// ���� ���̽��� ��ӹ޴� �ڽĵ鿡�� ������ �ش�Ǵ� ������ �ۼ��Ѵ�.
			// ���� cbuffer setup, sampler setup ��.
		virtual void Render() abstract;
		//virtual GraphicsEngineSpace::PrefabBase* GetPrefab() { return prefab; }
		virtual void PrepareRender(float tick) abstract;
		//virtual void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine>graphicsEngine) abstract;
		virtual void BoundingRender() {}
	};
}


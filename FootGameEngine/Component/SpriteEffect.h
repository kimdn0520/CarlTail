#pragma once
#include "RendererBase.h"
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{

	// 2D ��������Ʈ ����Ʈ�� �̾Ƴ��� ���� ������ ������Ʈ �Դϴ�.
	class SpriteEffect : public RendererBase
	{
	public:
		SpriteEffect(std::weak_ptr<GameObject> object);
		virtual ~SpriteEffect() {}


	public:
		// ���⼭ �ʱ�ȭ �� �� �̸��� �ֽ��ϴ�
		void Init(std::string objName);

		void Render() override;
		// Update
		void PrepareRender(float tick) override;

		void DebugIMGUIRender(float tick) override;
	};

}

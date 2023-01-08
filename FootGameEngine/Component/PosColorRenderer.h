#pragma once
#include "RendererBase.h"
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// �⺻���� PosColor�� �̾Ƴ��� ���� ������ ������Ʈ..
		// ��� ���Ŀ� �׷��Ƚ��� ������ ���� �̰� Mesh������ ������ ���� ���� ���� �� �ϴ�.
	class PosColorRenderer : public RendererBase
	{
	public:
		PosColorRenderer(std::weak_ptr<GameObject> object);

		virtual ~PosColorRenderer() {}

	public:
		// ���⼭ �ʱ�ȭ �� ��, ������Ʈ �̸��� �־��ش�.
			// ������ ���ؼ� �ʱ�ȭ�� ���̱� ������ ������ �̸��� �־��.
		void Init(std::string _objName);

		void Render() override;
		void PrepareRender(float tick) override;
	};

}
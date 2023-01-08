#pragma once
#include "RendererBase.h"
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// �� ���� Mesh �������� ���� �� �ִ�.
		// ���� �� ������ Ư�� ���̴��� ����ϴ� Ư�� ������ �����ͼ� ����� ������ ������ �̷��� �ϴ� ��..
	class MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer(std::weak_ptr<GameObject> object);

		virtual ~MeshRenderer() {}

	public:
		// ������ �ʿ��� Init �Լ�.. �������� ���ø��� �������.
		void Init(std::string objName);

		void Render() override;
		void PrepareRender(float tick) override;

		void PlayAnim(std::string animCilpName, bool isLoop);
	};
}
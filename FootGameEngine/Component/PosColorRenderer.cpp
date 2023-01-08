#include "GamePch.h"
#include "PosColorRenderer.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Camera.h"
#include "Object/GameObject.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"

using namespace GraphicsEngineSpace;

namespace GameEngineSpace
{
	PosColorRenderer::PosColorRenderer(std::weak_ptr<GameObject> object)
		: RendererBase(object)
	{
	}

	void PosColorRenderer::Init(std::string _objName)
	{
		// �� �������� ���� ����
		renderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
		BuilderManger::GetInstance()->GetBuilder("LineBuilder"), _objName);

		// �׷����� ������Ʈ�� �־��ݴϴ�.
		GraphicsManager::GetInstance()->GetRenderer()->AddRenderObj(renderObj);
	}

	void PosColorRenderer::Render()
	{
		//renderObj->Render();
	}

	void PosColorRenderer::PrepareRender(float tick)
	{
		// �� �Ŵ������� ���ͼ� ������Ʈ ������Ʈ
		std::shared_ptr<Camera> mainCam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
	
		renderObj->Update(transform->GetWorldTM(), mainCam->GetViewMatrix(), mainCam->GetProjMatrix());
	}
}

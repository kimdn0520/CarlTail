#pragma once
#include "Define/FootEngineTypedef.h"
//#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	class GameObject;

	class Camera;

	class RendererBase;

	class ColliderBase;

	class Animator;

	/**
	 * \brief ���������� ���� ������Ʈ ���� ������ �ִ� ScenceBase
	 *
	 * �⺻������ ���� ��ġ�� ���� ������Ʈ�鸸�� update, Render �� �� �ִ�.
	 *
	 * Ŭ���̾�Ʈ �������� �ش� ���̽��� ��ӹ��� ������ ���� ���� ������ �ۼ��� �� �ִ�.
	 *
	 * 2022.07.25(��) Dev.BlackFoot
	 */
	class SceneBase : public std::enable_shared_from_this<SceneBase>
	{
		tstring sceneName;

		std::shared_ptr<Camera> mainCamera;

		std::vector<std::shared_ptr<GameObject>> gameObjectInScene;

		// ������ ���� ������Ʈ ��Ƶ� ����
		std::vector<std::shared_ptr<RendererBase>> renderObjInScene;

		// �浹 ���� ������Ʈ ��Ƶ� ����
		std::vector<std::shared_ptr<ColliderBase>> colliderObjInScene;

		// ����Ʈ ����Ʈ
		//std::vector<GraphicsEngineSpace::PointLight*> pointLights;

		// ��ī�� �ڽ��� �޾��ݽô�.
			// �׷��Ƚ� ������ ���ҽ� �Ŵ������� �������ֱ� ������ �����͸� ������ش�.
		//GraphicsEngineSpace::SkyBoxBase* skyBox;
		// IBL�� ���� ����
		//GraphicsEngineSpace::IBLTexture* ibl;

		// ����� ������ �� �Լ� => ����ƽ���� ���� ��� ������ �����ϰ�.
		static bool debugRender;

	public:
		SceneBase(tstring sceneName = TEXT("SampleScene"));

		virtual ~SceneBase();
		// ���� �̺�Ʈ �Լ�..
			// ��κ��� ��� ������ ������Ʈ�� �Լ����� �ҷ��ش�.
		void Awake();
		void Start();
		void PreUpdate(float tick = 0.0f);
		void Update(float tick = 0.0f);
		void LateUpdate(float tick = 0.0f);

		// ��� ������ ������Ʈ�� ������(�׷��� ����)�� �Ѱ��ش�.
		void Render(float tick);

		void OnEnable();

		// ť��� ����
		/*void SetSkyBox(const tstring& skyBoxName);
		void SetIBL(const tstring& iblName, const tstring& radiancePath, const tstring& irradiancePath);*/


		// ī�޶� ����
		void SetMainCamera(std::shared_ptr<Camera> _camera) { mainCamera = _camera; }
		std::shared_ptr<Camera> GetMainCamera() { return mainCamera; }

		std::vector<std::shared_ptr<GameObject>> GetGameObjectInScene() { return gameObjectInScene; }

		//
		//GraphicsEngineSpace::PointLight* CreatePointLight();

		virtual void BuildScene() abstract;

		void ClearScene();

	private:
		void AddGameObject(std::shared_ptr<GameObject> _newGameObj);

	public:
		// �� ���� ������Ʈ�� �����Ѵ�.
		std::shared_ptr<GameObject> CreateEmpty();

		// ���� Ư�� ���� ������Ʈ�� �������� �ݶ��̴��� �߰��ϴ� �Լ��� �־��ش�
		// �ش� ������Ʈ�� ������ �ִ� ���ӿ�����Ʈ�� Awake�� �ҷ�����.
		void AddRenderer(std::shared_ptr<RendererBase> renderObj);
		void AddCollider(std::shared_ptr<ColliderBase> colliderObj);

		// �� �̸� ȹ��
		tstring GetSceneName() { return sceneName; }

		// SceneManager�� ���� �����ϱ� ���� friend Ŭ���� ����
		friend class SceneManager;
	};
}


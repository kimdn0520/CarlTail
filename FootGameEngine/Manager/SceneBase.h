#pragma once
#include "Define/FootEngineTypedef.h"
#include "Util/ObjectPool.h"
//#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	class GameObject;

	class Camera;

	class RendererBase;

	class Collider;

	class Animator;

	class Light;

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
		std::vector<std::shared_ptr<Collider>> colliderObjInScene;

		// ����Ʈ ����Ʈ + ���� ����Ʈ
		std::vector<std::shared_ptr<Light>> pointLightInScene;
		std::vector<std::shared_ptr<Light>> spotLightInScene;

		// ��ī�� �ڽ��� �޾��ݽô�.
			// �׷��Ƚ� ������ ���ҽ� �Ŵ������� �������ֱ� ������ �����͸� ������ش�.
		//GraphicsEngineSpace::SkyBoxBase* skyBox;
		// IBL�� ���� ����
		//GraphicsEngineSpace::IBLTexture* ibl;

		// ����� ������ �� �Լ� => ����ƽ���� ���� ��� ������ �����ϰ�.
		static bool debugRender;

		float fixedTimeStep;

	protected:
		std::shared_ptr<ObjectPool> objectPool;

	public:
		SceneBase(tstring sceneName = TEXT("SampleScene"));

		virtual ~SceneBase();
		// ���� �̺�Ʈ �Լ�..
			// ��κ��� ��� ������ ������Ʈ�� �Լ����� �ҷ��ش�.
		void Awake();
		void Start();
		void FixedUpdate(float tick);
		void PreUpdate(float tick = 0.0f);
		void Update(float tick = 0.0f);
		void LateUpdate(float tick = 0.0f);

		// ��� ������ ������Ʈ�� ������(�׷��� ����)�� �Ѱ��ش�.
		void Render(float tick);
		
		void OnEnable();

		// ī�޶� ����
		void SetMainCamera(std::shared_ptr<Camera> _camera) { mainCamera = _camera; }
		std::shared_ptr<Camera> GetMainCamera() { return mainCamera; }

		std::vector<std::shared_ptr<GameObject>> GetGameObjectInScene() { return gameObjectInScene; }
		std::shared_ptr<GameObject> FindObject(std::string uid);

		//
		//GraphicsEngineSpace::PointLight* CreatePointLight();

		virtual void BuildScene() abstract;

		void ClearScene();

	private:
		void AddGameObject(std::shared_ptr<GameObject> _newGameObj);

		// TODO : ������ ���� �ְ� ����
		std::shared_ptr<GameObject> InstantiateGameObject(std::shared_ptr<GameObject> _newGameObj);

		// Debug Render �κ��� ���� ���ô�..
		void DebugRender(float tick);

	public:
		// �� ���� ������Ʈ�� �����Ѵ�.
		std::shared_ptr<GameObject> CreateEmpty();

		// ���� Ư�� ���� ������Ʈ�� �������� �ݶ��̴��� �߰��ϴ� �Լ��� �־��ش�
		// �ش� ������Ʈ�� ������ �ִ� ���ӿ�����Ʈ�� Awake�� �ҷ�����.
		void AddRenderer(std::shared_ptr<RendererBase> renderObj);
		void AddCollider(std::shared_ptr<Collider> colliderObj);
		void AddPointLight(std::shared_ptr<Light> pLight);
		void AddSpotLight(std::shared_ptr<Light> sLight);

		// debug ���� ����
		void SetDebugRender(bool debugRender) { this->debugRender = debugRender; }
		bool GetDebugRender() { return debugRender; }

		// �� �̸� ȹ��
		tstring GetSceneName() { return sceneName; }

		// SceneManager�� ���� �����ϱ� ���� friend Ŭ���� ����
		friend class SceneManager;
	};
}


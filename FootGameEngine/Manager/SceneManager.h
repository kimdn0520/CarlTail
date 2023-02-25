#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class SceneBase;
	class PhysicsManager;
	class GameObject;

	/**
	 * \brief Scene�� �����ϴ� SceneManager Ŭ����
	 *
	 * �ش� �Ŵ��� Ŭ������ ������ ���ӿ������� ������ �ִ�. �̱������� �����Ѵ�.
	 *
	 * 2022.07.26(ȭ) Dev.BlackFoot
	 */
	class SceneManager final
	{
		// �� �ϳ� �����ϴ� SceneManager ��ü
		static std::shared_ptr<SceneManager> instance;

		// �����Ŵ��� ĳ�̿뵵
		std::shared_ptr<PhysicsManager> physicsManager;

	private:
		// ���� ��
		std::shared_ptr<SceneBase> currentScene;
		// ���� ��ü�� Scene
		std::map<tstring, std::shared_ptr<SceneBase>> scenes;

		// �������� �ε� �� �� ���� Ȯ�ο� ����
		bool reservedLoadScene;
		tstring reservedLoadSceneName;

		// Tick ����� delta�� ������ float����.
		float deltaMultiple;
		// ���� ƽ�� ������ �ִ°� �˾ƺ��� �Һ���
		bool isChangedTick = false;

		bool isChangeScene = false;

	public:
		SceneManager() = default;

		~SceneManager() = default;

		// �̵�, ���� ������ ����
		SceneManager(const SceneManager & _other) = delete;
		SceneManager& operator= (const SceneManager & _other) = delete;
		SceneManager(SceneManager && _other) = delete;
		SceneManager& operator= (SceneManager && _other) = delete;
		
		static std::shared_ptr<SceneManager> GetInstance();

		void Init();

		void Release();

		void Update();

		void AddGameScene(std::shared_ptr<SceneBase> _scene);

		std::shared_ptr<SceneBase> GetCurrentScene() { return currentScene; }

		void LoadScene(tstring _sceneName);

		// �������� ��..
		void UpdateMainCameraAspectRatio(uint32 _screenWidth, uint32 _screenHeight);

		// deltaTime ����
		void ChangeTick(float multiple);
		// delatTime ���� ����
		void SetTickOrigin() { isChangedTick = false; }

		void LoadSceneData(std::string sceneName);

		std::shared_ptr<GameObject> FindObject(std::string uid);
		std::shared_ptr<GameObject> FindObjectName(tstring name);

	private:
		void ChangeScene();
	};
}

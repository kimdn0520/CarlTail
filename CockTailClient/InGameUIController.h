#pragma once

namespace ClientSpace
{

	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	// ��ȭ ���¿� ���� ��Ÿ���� enumClass
	enum class eDialogState
	{
		FIRST_CUT_SCENE,	// ù �� ��

		STAGESTART,			// �������� ����

		Bar,				// bar 

		TUTORIAL,			// Ʃ�丮�� ����.

		NONE				// ��ȭâ �ȶߴ� ���� = default
	};

	enum class eBarEvent
	{
		FIRST_CUT_SCENE,
		Bar,
		NONE
	};


	struct Dialog
	{
		// �� �������� ��� �ߺ� ������ type
		uint32 typeID;
		std::wstring contents;

		Dialog(uint32 id, const std::wstring& content)
		{
			typeID = id;
			contents = content;
		}
	};


	class InGameUIController : public ComponentBase
	{

	public:
		InGameUIController(std::weak_ptr<GameObject> obj);
		~InGameUIController();

	private:
		std::shared_ptr<Canvas> mainCanvas;

	private:
		float tickCount;
		float moveSpeed;

		int currentHP;
		int maxHP;

		int currentBossHP;
		int bossMaxHP;

	public:
		// ���̵� �� , �ƿ� true false
		// false�϶� ����Ǵ°ǵ�, �ʱⰪ�� false�� �� ������ ���� �����Ҷ� ���̵������� ������...�װ� ������ true�� �ϸ��.
		bool stopIn = false;
		bool stopOut = true;


		// ���� ������..
		std::mt19937 engine;
		uniform_int_distribution<int> distribution;

	public:
		virtual void Awake() override;
		virtual void Start() override;

		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		void SceneCheck();

		void SetHP(int HP);
		void SetMaxHP(int maxHP);

		void SetBossHP(int HP);
		void SetBossMaxHP(int maxHP);

		void SetCustomer();

		void BarEvent(int tickCount);

		void FadeIn();
		void FadeOut();

		void StartNowDialog();
		void startBarDialog();

		void CombineItem();

	};

}


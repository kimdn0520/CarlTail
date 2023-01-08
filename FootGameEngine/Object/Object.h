#pragma once

/**
 * \brief �������� ������Ʈ ������ �����Ͽ� ���� ������Ʈ Ŭ����.
 */
namespace GameEngineSpace
{
	/**
	 * \brief 32��Ʈ�� ���� Ÿ���� static ������ ����
	 *
	 * ������Ʈ Ŭ������ ������ ������Ʈ�� ID�� �����ϴ� ID������ Ŭ�����Դϴ�.
	 */
	class ObjectIDGenerator
	{
		static uint32 nextID;

	public:
		// ���� ���� �����ڸ� ���ؼ� ���� �� ID�� ������Ų��.
		static uint32 IDGenerate() { return nextID++; }
	};

	/**
	 * \brief ���ӿ� �����ϴ� ��� ������Ʈ���� �ֻ��� �θ� Ŭ���� �Դϴ�.
	 *
	 * ���� ��ü ������ �����ϱ� ���� abstract Ű���带 ����߽��ϴ�.
	 */
	class Object abstract
	{
		// ���� ������Ʈ�� Ȱ��ȭ �Ǿ� �ִ°�.
		bool isEnable;

		// ������Ʈ ID
		uint32 objectID;

	public:
		Object();
		virtual ~Object();

		// ������Ʈ�� ��ӹ޴� ��� Ŭ������ ����� �ۼ�������Ѵ�.
		virtual void Release() abstract;

	public:
		static void Destroy();

		// ���� ��ȯ�ǵ� �������� ���� ������Ʈ�� �����ϴ� �Լ�
		static void DontDestroyOnLoad(std::shared_ptr<Object> targetObject);

		// Unity�� �̺�Ʈ �Լ��� ��������� �����Ͽ� ���� ������Ʈ ������ ����� ����� �Լ�.
			// �� �Լ����� ��� ������ ���� ���� ���� �ֱ� ������ abstract�� ������ ����.
		virtual void Awake();

		virtual void Start();

		virtual void PreUpdate(float tick);

		virtual void Update(float tick);

		virtual void LateUpdate(float tick);

		virtual void DebugIMGUIRender(float tick);

		// ������Ʈ�� Ȱ��ȭ �� �� �Ҹ��� �Լ�
		virtual void OnEnable();

		// �ݴ�� ��Ȱ��ȭ �� �� �Ҹ��� �Լ�
		virtual void OnDisable();

		virtual void OnDestroy();

		uint32 GetObjectID() const { return objectID; }

		bool GetIsEnable() const { return isEnable; }

		void SetIsEnable(bool _isEnable) { isEnable = _isEnable; }
	};
}
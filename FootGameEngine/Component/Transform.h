#pragma once
#include "Object/ComponentBase.h"

#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class GameObject;

	enum class Space
	{
		WORLD,
		LOCAL
	};

	/**
	 * \brief ��� ���� ������Ʈ�� �ݵ�� ������ �ִ� Ʈ������ ������Ʈ �Դϴ�.
	 *
	 * ������Ʈ�� ��ġ, ����, ������ ������ ������ �ֽ��ϴ�.
	 *
	 * 2022.07.22(��) Dev.BlackFoot
	 */
	class Transform : public ComponentBase
	{
		// ���� ��ġ ���� ������
		// Transform Matrix
			// Local TM == �θ��� �������� �θ� ���������� �ڱ� ��ġ�� �̵���Ű�� TM / LocalTM
			// World TM == ���� �������� World ��ġ�� ���� TM
		Matrix localTM;
		Matrix worldTM;

		// �⺻������ ���������� Vector�� w���� ���� ����ִ�..
		Vector3 localPosition;
		Vector3 localRotation;
		Vector3 localScale;

		Vector3 worldPosition;
		Vector3 worldRotation;
		Vector3 worldScale;

		// ������������, ��, �� ������ ����
		Vector3 look;
		Vector3 up;
		Vector3 right;

		// TM ������Ʈ�� ���� ���� ���� �� �Լ�.
		// ���� �ڽ��� TM�� ���� ������ϴ°�.
		bool needTMUpdate;

		float mousePosX;
		float mousePosY;

	public:
		// YAMLParser�κ��� ���� ID ����Ʈ
		std::vector<std::string> childIDList;

	public:
		Transform(std::weak_ptr<GameObject> _gameObject);

		virtual ~Transform();

		virtual void Release() override;

		virtual void Update(float tick) override;

		// ������ ��ġ ������, ����� �Ǵ� ������ �������ݴϴ�.
		void SetPosition(const Vector3& _newPos, Space relativeTo = Space::WORLD);
		void SetRotation(const Vector3& _eulerAngle, Space relativeTo = Space::WORLD);
		void SetRotation(const Quaternion& _quat, Space relativeTo = Space::WORLD);
		void AddRotation(const Vector3& _eulerAngle, Space relativeTo = Space::WORLD);
		// Scale�� ��� ���带 ��������� �ʽ��ϴ�..
		void SetScale(const Vector3& _newLocalScale);

		// ������ Private ���� ����.
		const Vector3& GetLocalPosition() { return localPosition; }
		const Vector3& GetLocalRotation() { return localRotation; }
		const Vector3& GetLocalScale() { return localScale; }

		const Vector3& GetWorldPosition() { return worldPosition; }
		const Vector3& GetWorldRotation() { return worldRotation; }
		const Vector3& GetWorldScale() { return worldScale; }

		//�߰��մϴ�
		const Quaternion& GetWorldRotationQuat();


		void LookAt(const Vector3& targetPos);

		const Vector3& GetLook() { return look; }
		void SetLook(const Vector3& _look);
		const Vector3& GetUp() { return up; }
		void SetUp(const Vector3& _up);
		const Vector3& GetRight() { return right; }
		void SetRight(const Vector3& _right);

		const Matrix& GetLocalTM();
		const Matrix& GetWorldTM();

		void SetLocalTM(const Matrix& _localTM);
		void SetWorldTM(const Matrix& _worldTM);

		// ���� �������� ȸ��
		void Rotate(const Vector3& rot);

		// ������ǥ�� �ڱ��ڽ�(local)���� �̵��Ѵ�.
		void TransLate(const Vector3& pos);

		// ���Ϸ��� ���ʹϾ�����~
		Vector4 EulerToQuat(Vector3 euler);
		// �ݴ뵵 �ʿ��ϴ� �̰ž�
		Vector3 QuatToEuler(Quaternion value);


	private:
		// ���� TM�� ����
		void UpdateLocalTM();
		// WorldTM�� ����
		void UpdateWorldTM();
		// needTM UPdate ���� True�� ����
		void CheckingNeedTMUpdate();

		// Decompose
		void DecomposeMatrix(Vector3& scale, Vector3& rotation, Vector3& position, Matrix matrix);
	};
}
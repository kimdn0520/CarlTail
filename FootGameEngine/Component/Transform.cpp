#include "GamePch.h"
#include "Transform.h"

#include "Object/GameObject.h"
// #include "Component/Camera.h"

namespace GameEngineSpace
{
	Transform::Transform(std::weak_ptr<GameObject> _gameObject) :
		ComponentBase(_gameObject, ComponentType::TRANSFORM)
		, localTM(Matrix::Identity)
		, worldTM(Matrix::Identity)
		, localPosition(Vector3::Zero)
		, localRotation(Vector3::Zero)
		, localScale(Vector3(1.0f, 1.0f, 1.0f))
		, worldPosition(Vector3::Zero)
		, worldRotation(Vector3::Zero)
		, worldScale(Vector3(1.0f, 1.0f, 1.0f))
		, look(Vector3::UnitZ)
		, up(Vector3::UnitY)
		, right(Vector3::UnitX)
		, needTMUpdate(true)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Release()
	{
		__super::Release();
	}

	void Transform::Update(float tick)
	{
		this->UpdateLocalTM();
		this->UpdateWorldTM();
	}

	void Transform::UpdateLocalTM()
	{
		// ���������� Local TM�� ������ݴϴ�.
			// SRT ������ ������ָ�, R�� ��� Z Roll X Pitch Y Yaw ������ ȸ�� �մϴ�.
		//localTM = MatrixScalingFromVector(localScale) * MatrixRotationFromVector(localRotation) * MatrixTranslationFromVector(localPosition);
		localTM = Matrix::CreateScale(localScale) * XMMatrixRotationQuaternion(EulerToQuat(localRotation)) * Matrix::CreateTranslation(localPosition);

		// ������ ����Ǿ��� ������.. TM ������Ʈ�� �ʿ��ϴٰ� üũ ���ݴϴ�.
			// ���� TM�� �����ϰ� �� �� TM ������Ʈ�� �ʿ� ���� �ʴٰ� ���ݴϴ�.
		CheckingNeedTMUpdate();
	}

	void Transform::UpdateWorldTM()
	{
		// �θ���� ���谡 �ݵ�� �ʿ�..
		// �θ��� ����TM�� �ڽ��� Local�� ������ �� => �ڽ��� WorldTM
		// �θ��� ���� ��ġ�� �ڽ��� ���� �������� ����.. �ű⼭ ���� TM ��ŭ �Ű��� ���� �ڽ��� World TM�̴�.

		// �θ� ���ͼ�.. �θ��� ���忡 �ڽ��� ������ �����ش�.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			// �θ� �����Ѵٸ�..
			// �θ��� ���忡 �ڽ��� ������ �����ݴϴ�.
			worldTM = localTM * (_parent->GetComponent<Transform>()->GetWorldTM());

			// ������ ��Ҹ� �и��ؼ� ������ ������, �����̼�, �����ǿ� �־��ݴϴ�.
			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}
		else
		{
			// �θ� ���� ���. ����TM�� �״�� WorldTM���� �־��ݴϴ�.
			worldTM = localTM;

			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}

		// look up right�� �������ݴϴ�.
		// World TM�� �������� �븻 Ʈ�������� ���ݴϴ�..
		Vector3::TransformNormal(Vector3::UnitZ, worldTM, look);
		Vector3::TransformNormal(Vector3::UnitY, worldTM, up);
		Vector3::TransformNormal(Vector3::UnitX, worldTM, right);
		/*right = Vector3::UnitX * worldTM;
		up = Vector3::UnitY * worldTM;
		look = Vector3::UnitZ * worldTM;

		// �� ���� ����ȭ
		right = Vector3Normalize(right);
		up = Vector3Normalize(up);
		look = Vector3Normalize(look);*/

		// WorldTM�� ���� �߱� ������ TM Update�� �ʿ����� �ʽ��ϴ�..
		needTMUpdate = false;
	}


	void Transform::CheckingNeedTMUpdate()
	{
		needTMUpdate = true;

		// �� ���� ������Ʈ�� ��� �ڽĵ��� TM ������Ʈ�� �ʿ��ϵ��� ���ݴϴ�.
		// ���� ����� ������Ʈ�� TM�� ����Ǿ��� ������ �ڿ������� �ڽĵ��� TM�� ����Ǿ�� �ϱ� �����Դϴ�.

		std::vector<std::shared_ptr<GameObject>>& children = this->GetGameObject()->GetChildren();

		// ��� �ڽ��� Ʈ�������� TM Update�� �ʿ��ϵ���!
		for (auto& child : children)
		{
			child->GetComponent<Transform>()->CheckingNeedTMUpdate();
		}
	}

	const DirectX::SimpleMath::Quaternion& Transform::GetWorldRotationQuat()
	{
		DirectX::SimpleMath::Quaternion _wRot = EulerToQuat(worldRotation);
		return _wRot;
	}

	void Transform::LookAt(const Vector3& targetPos)
	{
		Vector3 nowPos = Vector3{GetWorldTM().m[3][0], GetWorldTM().m[3][1], GetWorldTM().m[3][2]};
		if (targetPos - nowPos == Vector3::Zero)
			return;

		// Eye direction�� ��Ŀ�� ��ġ���� ���� ��ġ�� ���� ���� ��.
		// up direction���� look At ���
		Vector3 eyeDirection = targetPos - nowPos;

		// ���������� �ڵ�
		assert((eyeDirection == Vector3::Zero) != true);
		assert((isinf(eyeDirection.x) || isinf(eyeDirection.y) || isinf(eyeDirection.z)) != true);
		// UnitY�� �� ���̱� ������ up direction�� ���� ������ ���� ���� �ʴ´�.
		Vector3 _look;
		eyeDirection.Normalize(_look);

		Vector3 _right;
		Vector3::UnitY.Cross(_look, _right);
		_right.Normalize(_right);

		Vector3 _up;
		_look.Cross(_right, _up);

		Matrix matrix = Matrix{_right, _up, _look};
		matrix.Transpose(matrix);

		right = Vector3{ matrix.m[0][0], matrix.m[1][0], matrix.m[2][0] };
		up = -Vector3{ matrix.m[0][1], matrix.m[1][1], matrix.m[2][1] };
		look = Vector3{ matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] };

		// �� ���� ����ȭ
		right.Normalize(right);
		up.Normalize(up);
		look.Normalize(look);

		matrix.Invert(matrix);
		Vector3 rotation = matrix.ToEuler();

		rotation.x = XMConvertToDegrees(rotation.x);
		rotation.y = XMConvertToDegrees(rotation.y);
		rotation.z = XMConvertToDegrees(rotation.z);

		SetRotation(rotation, Space::LOCAL);
	}

	void Transform::SetLook(const Vector3& _look)
	{
		// Look�� �ٲ������ �������� �ٲ�����.
		look = _look;
		// �޼����̷� ����
		look.Normalize(look);

		// look�� ���� ���� �̿��ؼ� right�� �̾��ش�.
		Vector3::UnitY.Cross(look, right); // right = UnitY X look
		//look.Cross(Vector3::UnitY, right);
		//right = -right;
		right.Normalize(right);


		look.Cross(right, up);
		//right.Cross(look, up);
		//up = -up;
		up.Normalize(up);

		Matrix matrix = Matrix{ right, up, look };
		matrix.Transpose(matrix);

		right = Vector3{ matrix.m[0][0], matrix.m[1][0], matrix.m[2][0] };
		up = -Vector3{ matrix.m[0][1], matrix.m[1][1], matrix.m[2][1] };
		look = Vector3{ matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] };

		// �� ���� ����ȭ
		right.Normalize(right);
		up.Normalize(up);
		look.Normalize(look);

		matrix.Invert(matrix);
		Vector3 rotation = matrix.ToEuler();

		rotation.x = XMConvertToDegrees(rotation.x);
		rotation.y = XMConvertToDegrees(rotation.y);
		rotation.z = XMConvertToDegrees(rotation.z);

		SetRotation(rotation, Space::LOCAL);
	}

	// ���� �ڵ忡 ������ �ֽ��ϴ�..
		// Up ���Ϳ� Right���ͷ� ������ ���� ���͸� ���ϴ� ���� ������ �ִ°�..
	void Transform::SetUp(const Vector3& _up)
	{
		up = _up;
		up.Normalize(up);

		up.Cross(Vector3::UnitZ, right);
		right.Normalize(right);

		right.Cross(up, look);
		look.Normalize(look);
	}

	void Transform::SetRight(const Vector3& _right)
	{
		right = _right;
		right.Normalize(right);

		Vector3::UnitZ.Cross(right, up);
		up.Normalize(up);

		right.Cross(up, look);
		look.Normalize(look);
	}

	void Transform::DecomposeMatrix(Vector3& scale, Vector3& rotation, Vector3& position, Matrix matrix)
	{
		// Position, Scale
		position.x = matrix.m[3][0];
		position.y = matrix.m[3][1];
		position.z = matrix.m[3][2];

		scale.x = sqrtf(powf(matrix.m[0][0], 2.0f) + powf(matrix.m[0][1], 2.0f) + powf(matrix.m[0][2], 2.0f));
		scale.y = sqrtf(powf(matrix.m[1][0], 2.0f) + powf(matrix.m[1][1], 2.0f) + powf(matrix.m[1][2], 2.0f));
		scale.z = sqrtf(powf(matrix.m[2][0], 2.0f) + powf(matrix.m[2][1], 2.0f) + powf(matrix.m[2][2], 2.0f));

		Vector3 matrix0 = Vector3{ matrix.m[0] };
		Vector3 matrix1 = Vector3{ matrix.m[1] };
		Vector3 matrix2 = Vector3{ matrix.m[2] };

		Vector3 def;
		matrix0.Cross(matrix1, def);

		// Negative Scale
		if (def.Dot(matrix2) < 0.0f)
		{
			scale *= -1.0f;
		}

		// Rotation
		matrix0 /= scale.x;
		matrix1 /= scale.y;
		matrix2 /= scale.z;

		Matrix rotMatrix = Matrix{matrix0, matrix1, matrix2};

		rotation = rotMatrix.ToEuler();

		rotation.x = XMConvertToDegrees(rotation.x);
		rotation.y = XMConvertToDegrees(rotation.y);
		rotation.z = XMConvertToDegrees(rotation.z);
	}

	void Transform::SetPosition(const Vector3& _newPos, Space relativeTo)
	{
		if (relativeTo == Space::WORLD)
		{
			// ���� ��ü�� �������� ������ ���
			worldPosition = _newPos;

			// ���� �������� ���� �Ǿ�����, �θ����忡�� �� ���� �����ǵ� ���� �Ǿ���.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector3 _parentPos = Vector3::Zero;

			// �θ� ������
			if (_parent != nullptr)
			{
				_parentPos = _parent->GetComponent<Transform>()->GetWorldPosition();
			}

			// ���� ������ ��ġ���� �θ��� ���� ��ġ�� ���ذ�, �ڽ��� ���� ��ġ�� �ȴ�.
			Vector3 _newLocalPos = _newPos - _parentPos;

			localPosition = _newLocalPos;
		}
		else
		{
			localPosition = _newPos;
		}

		// ���� ������ update
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::SetRotation(const Vector3& _eulerAngle, Space relativeTo)
	{
		// Position�� ���������� ���ð� ���忡 ���� ������ ���ش�.
		if (relativeTo == Space::WORLD)
		{
			worldRotation = _eulerAngle;

			// ���带 �ٲ� �� ������ �ٲ��ش�.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector3 _parentEulerAngle = Vector3::Zero;

			// �θ� ���� Ȯ��
			if (_parent != nullptr)
			{
				_parentEulerAngle = _parent->GetComponent<Transform>()->GetWorldRotation();
			}

			Vector3 _newLocalRot = _eulerAngle - _parentEulerAngle;

			localRotation = _newLocalRot;
		}
		else
		{
			localRotation = _eulerAngle;
		}

		// ������ Ư�� ������, ȸ����, ��ġ�� ����Ǹ� �ٷ� TM�� �������ش�.
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::SetRotation(const Quaternion& _quat, Space relativeTo /*= Space::WORLD*/)
	{
		Vector3 _eular = QuatToEuler(_quat);

		if (relativeTo == Space::WORLD)
		{
			worldRotation = _eular;

			// ���带 �ٲ� �� ������ �ٲ��ش�.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector3 _parentEulerAngle = Vector3::Zero;

			// �θ� ���� Ȯ��
			if (_parent != nullptr)
			{
				_parentEulerAngle = _parent->GetComponent<Transform>()->GetWorldRotation();
			}

			Vector3 _newLocalRot = _eular - _parentEulerAngle;

			localRotation = _newLocalRot;
		}
		else
		{
			localRotation = _eular;
		}

		// ������ Ư�� ������, ȸ����, ��ġ�� ����Ǹ� �ٷ� TM�� �������ش�.
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::AddRotation(const Vector3& _eulerAngle, Space relativeTo)
	{
		this->SetRotation(this->GetLocalRotation() + _eulerAngle);
	}

	void Transform::SetScale(const Vector3& _newLocalScale)
	{
		localScale = _newLocalScale;

		UpdateLocalTM();
	}

	const Matrix& Transform::GetLocalTM()
	{
		return localTM;
	}

	const Matrix& Transform::GetWorldTM()
	{
		// TM Update�� üũ �Ǿ�������.. ������Ʈ �ϰ� ��ȯ���ش�.
		if (needTMUpdate)
		{
			UpdateWorldTM();
		}

		return worldTM;
	}

	void Transform::SetLocalTM(const Matrix& _localTM)
	{
		localTM = _localTM;

		// ������ ������ �� ������ ũ�� ���� ��ġ�� Transpose���ݴϴ�
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		// ������ ����Ǿ�����.. TMUpdate�� �ʿ��ϴٰ� üũ ���ݴϴ�.
		CheckingNeedTMUpdate();
	}

	void Transform::SetWorldTM(const Matrix& _worldTM)
	{
		worldTM = _worldTM;

		// ���尡 �ٲ������.. ���� ���� �������ݴϴ�.
			// ������ �θ�� �����Ǿ� �ֱ� ������ �θ� ã���ݴϴ�.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			Matrix _parentWorld = _parent->GetComponent<Transform>()->GetWorldTM();

			// �θ��� ���� ������� �ڽ��� ���忡 �����ָ� ������ ���ɴϴ�.
				// => �ڽ��� Local * �θ��� World = �ڽ��� World
				// => �ڽ��� Local == �ڽ��� World * �θ��� World�� �����
			_parentWorld.Invert(_parentWorld);
			localTM = worldTM * _parentWorld;
		}
		else
		{
			localTM = worldTM;
		}

		// ������ Transpose���ݴϴ�.
		DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		CheckingNeedTMUpdate();
	}

	void Transform::Rotate(const Vector3& rot)
	{
		localRotation += rot;

		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::TransLate(const Vector3& pos)
	{
		localPosition += pos;

		UpdateLocalTM();
		UpdateWorldTM();
	}

	Vector4 Transform::EulerToQuat(Vector3 euler)
	{
		Vector4 result;

		float x = XMConvertToRadians(euler.x) * 0.5f;
		float y = XMConvertToRadians(euler.y) * 0.5f;
		float z = XMConvertToRadians(euler.z) * 0.5f;

		float sinX = sinf(x);
		float cosX = cosf(x);
		float sinY = sinf(y);
		float cosY = cosf(y);
		float sinZ = sinf(z);
		float cosZ = cosf(z);

		result.x = (cosY * sinX * cosZ) + (sinY * cosX * sinZ);
		result.y = (sinY * cosX * cosZ) - (cosY * sinX * sinZ);
		result.z = (cosY * cosX * sinZ) - (sinY * sinX * cosZ);
		result.w = (cosY * cosX * cosZ) + (sinY * sinX * sinZ);

		return result;
	}

	Vector3 Transform::QuatToEuler(Quaternion value)
	{
		Vector3 rotation;

		double x = value.x;
		double y = value.y;
		double z = value.z;
		double w = value.w;
		double sqx = x * x;
		double sqy = y * y;
		double sqz = z * z;
		double sqw = w * w;
		float exceptCheck = 2.0f * (w * x - y * z);
		float eulerX = 0.f;

		if (abs(exceptCheck) >= 1.f)
			eulerX = copysign(3.141592f / 2, exceptCheck);
		else
			eulerX = asin(2.0f * (w * x - y * z));

		float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
		float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));
		rotation = Vector3{ eulerX, eulerY, eulerZ };

		rotation = Vector3{ XMConvertToDegrees(rotation.x),
					   XMConvertToDegrees(rotation.y),
					   XMConvertToDegrees(rotation.z) };

		return rotation;
	}
}
#pragma once

namespace GameEngineSpace
{
	/// �浹 ó���� ���� �ٿ�� ���� ����ü���� �����ص� ��� ����.
	///	3���� ť��, 2���� Rect, 3���� ���, 3���� ����ü�� ���� bounding volume�� �ִ�.

	/// <summary>
	/// 3���� ť�� �ٿ�� ����
	/// </summary>
	struct BoundingCube
	{
		// �ڽ��� �������� ���� ���� ������
		Vector3 localScale;
		// �ڽ��� �������� ���� ���� �߽�(��ġ ����)
		Vector3 localCenter;
		// �ڽ��� �������� ���� ���� ��Ʈ����(ȸ�� ���� ���..?)
		Matrix localMatrix;
		// �� ���� ��ġ ����.
		Vector3 localPoint[8];

		// �߽��� �������� scale ��ŭ�� ũ��� ������ ��´�.
		BoundingCube(const Vector3& _localScale, const Vector3& _localCenter, const Matrix& _localMatrix)
			: localScale(_localScale)
			, localCenter(_localCenter)
			, localMatrix(_localMatrix)
		{
			// ������ ũ�⸦ �������� ������ ��´�.
			float halfX = localScale.x / 2;
			float halfY = localScale.y / 2;
			float halfZ = localScale.z / 2;

			// ť�긦 �̷�� ������ ��ġ�� �ʱ�ȭ �����ݴϴ�.
			localPoint[0] = localCenter + Vector3{ -halfX, -halfY, -halfZ };
			localPoint[1] = localCenter + Vector3{ -halfX, halfY, -halfZ };
			localPoint[2] = localCenter + Vector3{ halfX, -halfY, -halfZ };
			localPoint[3] = localCenter + Vector3{ -halfX, -halfY, halfZ };
			localPoint[4] = localCenter + Vector3{ halfX, -halfY, halfZ };
			localPoint[5] = localCenter + Vector3{ halfX, halfY, -halfZ };
			localPoint[6] = localCenter + Vector3{ -halfX, halfY, halfZ };
			localPoint[7] = localCenter + Vector3{ halfX, halfY, halfZ };

		}
	};

	// �Ϲ����� 2���� rect
	struct BoundingRect
	{
		// lefttop�� rightbottom
		Vector2 leftTop;
		Vector2 rightBottom;

		BoundingRect(Vector2 _leftTop, Vector2 _rightBottom)
			: leftTop(_leftTop)
			, rightBottom(_rightBottom)
		{

		}
		BoundingRect(Vector4 rect)
			: leftTop(rect.x, rect.y)
			, rightBottom(rect.z, rect.w)
		{
		}
	};

	// 3���� ���
	struct BoundingPlane
	{
		Vector3 normal;

		float distance;

		BoundingPlane()
			: normal(Vector3::Zero)
			, distance()
		{

		}

		// 3�������� �� �� ���� �־����� �� ����� ������ݴϴ�.
		BoundingPlane(Vector3 first, Vector3 second, Vector3 third)
		{
			// ù ������ second�� ���ϴ� ����
			Vector3 e1 = first - second;
			Vector3 e2 = first - third;

			// �ݽð�� ������ �̾Ƴ��ϴ�. CCW
				// => �ش� ���� �� ����� �븻�� �˴ϴ�.
			e1.Cross(e2, normal);
			normal.Normalize(normal);

			// ������� �ǹ��� dot����
				// ����� ������ => ax + by + cz = - d;
				// ���⼭ a, b, c�� normal, x, y, z�� first
			distance = -normal.Dot(first);
		}
	};

	// �ٿ�� �������� (����ü..!)
	struct BoundingFrustum
	{
		BoundingPlane planes[6];

		// far plane�� �ڳ� �� ��ǥ
			// lefttop�� x, y, z, 1
			// rightTop�� x, y, z, 1
			// leftbottom�� x, y, z, 1
			// rightbottom�� x, y, z, 1.
		Vector4 frustumCorner[4];
	};
}

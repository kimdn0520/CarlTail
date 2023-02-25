#pragma once
#include "Collider.h"
#include "PhysicsManager.h"

namespace GameEngineSpace
{
	class Camera;
	/**
	 * \brief �׺���̼� �޽ÿ� �۵��ϴ� ������Ʈ
	 * Recast & Detour Library
	 * �̷��� �ϴ����� �𸣰ڴµ� Recast & Detour Tool���� NavMesh�� �̰ԵǸ� ������ ��ǥ��� �� �������־
	 * start, end �� Change���ְ� ��θ� ���Ѵ����� �ش� WayPoint�� Change ���༭ ���� ���ߴ�.
	 * ������ NavMesh ���� vertex���� �̸� �������ָ� ���� ������ �����ؼ� �����غ������� vertex�� �ٲ㼭�� ������ �����ʾҴ�.
	 * �ش� Ÿ�ϵ��� ���� ������ �� �ٲ���� �ϴ°� ������ �ƹ��� ã�ƺ��� �ذ��� �� ������.
	 * 2023.01.17(��) Dev.Newbie
	 */
	class NavMeshAgent : public ComponentBase
	{
	private:	
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Rigidbody> rigidbody;

		static const int MAX_POLYS = 256;
		static const int MAX_SMOOTH = 2048;
		static const int MAX_AGENTS = 100;

		// �������� �𸣴� ������..
		// Agent Size
		float radius;
		float height;
		float baseOffset;		// ��ȯ �ǹ����� �������� �浹 ������ �������Դϴ�.

		// Steering
		float speed;			// �ִ� �̵� �ӵ�
		float rotateSpeed;
		float angularSpeed;		// �ִ� ���ӵ�
		float acceleration;		// �ִ� ���ӵ�
		float stopDistance;		// Agent�� ��ǥ�� �����ϴٰ� ��ǥ ��ġ�� ����������� ������ �����ϴ� ���� �Ÿ�
		bool autoBreaking;		// Ȱ��ȭ�Ǹ� ������Ʈ�� ��� ������ �� �ӵ��� ������

		bool isEnabled;			// NavMeshAgent ������ ���θ� ��ֹ��� �ν���. �׷��� �̰� ������ ���߰� ������Ʈ��ü�� ��Ȱ��ȭ ���ش�.

		bool isTrace;			// ���� ����

		SimpleMath::Vector3 destination;

		SimpleMath::Vector3 currentDestination;

		bool isChangeDestination;

		dtPolyRef polys[MAX_POLYS];
		int npolys;
		float polyPickExt[3];	// �� ���� Ž�� �Ÿ�

		float smoothPath[MAX_SMOOTH * 3];
		int nsmoothPath;

		float straightPath[MAX_POLYS * 3];					// ������ ���� ���
		unsigned char straightPathFlags[MAX_POLYS];
		dtPolyRef straightPathPolys[MAX_POLYS];
		int straightPathCount;

		std::vector<SimpleMath::Vector3> wayPoint;
		int wayPointIndex;
	
		SimpleMath::Matrix rotMatrix = XMMatrixIdentity();

		RaycastHit hitSlope;
		float slopeAngle;
		float maxSlopeAngle;
		Vector3 gravity;

	public:
		NavMeshAgent(std::weak_ptr<GameObject> _gameObject);
		virtual ~NavMeshAgent();

	public:
		inline SimpleMath::Vector3 ChangeCoordinate(float x, float y, float z);

		void SetDestination(SimpleMath::Vector3 pos);

		void TrackingStatus(bool _isTrace) { isTrace = _isTrace; }

		void SetSpeed(float _speed) { speed = _speed; }

		std::vector<SimpleMath::Vector3> GetWayPoint() { return wayPoint; }

		int GetWayPointIndex() { return wayPointIndex; }

		void Awake() override;

		void DebugIMGUIRender(float tick) override;

		void Update(float tick) override;

		bool IsSlope();
		
		bool IsGround();

		SimpleMath::Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);

		void Release() override;
	};
}

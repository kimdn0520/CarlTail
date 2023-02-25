#include "GamePch.h"
#include "NavMeshAgent.h"
#include "Transform.h"
#include "Component/Camera.h"
#include "SceneBase.h"
#include "Object/GameObject.h"

#include "WindowManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "NavigationManager.h"

namespace GameEngineSpace
{
	NavMeshAgent::NavMeshAgent(std::weak_ptr<GameObject> _gameObject)
		: ComponentBase(_gameObject, ComponentType::NAVAGENT)
		, radius(0.5f)
		, height(2.0f)
		, baseOffset(1.0f)
		, speed(2.5f)
		, rotateSpeed(30.f)
		, angularSpeed(120.f)
		, acceleration(8.0f)
		, stopDistance(0.f)
		, wayPointIndex(0)
		, maxSlopeAngle(55.f)
		, gravity(Vector3(0.f, 0.f, 0.f))
		, autoBreaking(true)
		, isTrace(true)
		, isChangeDestination(true)
		, destination(SimpleMath::Vector3(0.f, 0.f, 0.f))
		, currentDestination(SimpleMath::Vector3(0.f, 0.f, 0.f))
	{
		// Ʈ������ ĳ��
		transform = GetGameObject()->GetComponent<Transform>();

		camera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		polyPickExt[0] = 2;
		polyPickExt[1] = 4;
		polyPickExt[2] = 2;

		const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

		const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

		DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

		rotMatrix *= XMMatrixRotationQuaternion(q);
	}

	NavMeshAgent::~NavMeshAgent()
	{}

	inline bool inRange(const float* v1, const float* v2, const float r, const float h)
	{
		const float dx = v2[0] - v1[0];
		const float dy = v2[1] - v1[1];
		const float dz = v2[2] - v1[2];
		return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
	}

	static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
		const dtPolyRef* visited, const int nvisited)
	{
		int furthestPath = -1;
		int furthestVisited = -1;

		// Find furthest common polygon.
		for (int i = npath - 1; i >= 0; --i)
		{
			bool found = false;
			for (int j = nvisited - 1; j >= 0; --j)
			{
				if (path[i] == visited[j])
				{
					furthestPath = i;
					furthestVisited = j;
					found = true;
				}
			}
			if (found)
				break;
		}

		// If no intersection found just return current path. 
		if (furthestPath == -1 || furthestVisited == -1)
			return npath;

		// Concatenate paths.	

		// Adjust beginning of the buffer to include the visited.
		const int req = nvisited - furthestVisited;
		const int orig = rcMin(furthestPath + 1, npath);
		int size = rcMax(0, npath - orig);
		if (req + size > maxPath)
			size = maxPath - req;
		if (size)
			memmove(path + req, path + orig, size * sizeof(dtPolyRef));

		// Store visited
		for (int i = 0; i < req; ++i)
			path[i] = visited[(nvisited - 1) - i];

		return req + size;
	}

	// This function checks if the path has a small U-turn, that is,
	// a polygon further in the path is adjacent to the first polygon
	// in the path. If that happens, a shortcut is taken.
	// This can happen if the target (T) location is at tile boundary,
	// and we're (S) approaching it parallel to the tile edge.
	// The choice at the vertex can be arbitrary, 
	//  +---+---+
	//  |:::|:::|
	//  +-S-+-T-+
	//  |:::|   | <-- the step can end up in here, resulting U-turn path.
	//  +---+---+
	static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
	{
		if (npath < 3)
			return npath;

		// Get connected polygons
		static const int maxNeis = 16;
		dtPolyRef neis[maxNeis];
		int nneis = 0;

		const dtMeshTile* tile = 0;
		const dtPoly* poly = 0;
		if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
			return npath;

		for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
		{
			const dtLink* link = &tile->links[k];
			if (link->ref != 0)
			{
				if (nneis < maxNeis)
					neis[nneis++] = link->ref;
			}
		}

		// If any of the neighbour polygons is within the next few polygons
		// in the path, short cut to that polygon directly.
		static const int maxLookAhead = 6;
		int cut = 0;
		for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
			for (int j = 0; j < nneis; j++)
			{
				if (path[i] == neis[j]) {
					cut = i;
					break;
				}
			}
		}
		if (cut > 1)
		{
			int offset = cut - 1;
			npath -= offset;
			for (int i = 1; i < npath; i++)
				path[i] = path[i + offset];
		}

		return npath;
	}

	static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
		const float minTargetDist,
		const dtPolyRef* path, const int pathSize,
		float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
		float* outPoints = 0, int* outPointCount = 0)
	{
		// Find steer target.
		static const int MAX_STEER_POINTS = 3;
		float steerPath[MAX_STEER_POINTS * 3];
		unsigned char steerPathFlags[MAX_STEER_POINTS];
		dtPolyRef steerPathPolys[MAX_STEER_POINTS];
		int nsteerPath = 0;
		navQuery->findStraightPath(startPos, endPos, path, pathSize,
			steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
		if (!nsteerPath)
			return false;

		if (outPoints && outPointCount)
		{
			*outPointCount = nsteerPath;
			for (int i = 0; i < nsteerPath; ++i)
				dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
		}


		// Find vertex far enough to steer to.
		int ns = 0;
		while (ns < nsteerPath)
		{
			// Stop at Off-Mesh link or when point is further than slop away.
			if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
				!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
				break;
			ns++;
		}
		// Failed to find good point to steer to.
		if (ns >= nsteerPath)
			return false;

		dtVcopy(steerPos, &steerPath[ns * 3]);
		steerPos[1] = startPos[1];
		steerPosFlag = steerPathFlags[ns];
		steerPosRef = steerPathPolys[ns];

		return true;
	}

	inline SimpleMath::Vector3 NavMeshAgent::ChangeCoordinate(float x, float y, float z)
	{
		DirectX::XMFLOAT3 vert = { x, z, y };
		DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vert);
		vec = DirectX::XMVector3Transform(vec, rotMatrix);

		return vec;
	}

	void NavMeshAgent::SetDestination(SimpleMath::Vector3 pos)
	{
		destination = pos;
	}

	void NavMeshAgent::Awake()
	{
		rigidbody = GetGameObject()->GetComponent<Rigidbody>();
	}

	void NavMeshAgent::DebugIMGUIRender(float tick)
	{
		if (wayPoint.empty())
			return;

		if (GraphicsEngineSpace::ImGUIManager::GetInstance()->DrawBegin("DrawPath" + std::to_string(GetObjectID())))
		{
			std::vector<SimpleMath::Vector4> wayPointVector4;

			for (auto& way : wayPoint)
			{
				wayPointVector4.push_back({ way.x, way.y, way.z, 1.0f });
			}

			XMMATRIX viewProj = XMMatrixMultiply(camera->GetViewMatrix(), camera->GetProjMatrix());

			for (int i = wayPointIndex; i < wayPointVector4.size() - 1; i++)
			{
				Vector4 screenPos = XMVector4Transform(XMLoadFloat4(&wayPointVector4[i]), viewProj);
				Vector4 screenPos2 = XMVector4Transform(XMLoadFloat4(&wayPointVector4[i + 1]), viewProj);

				// Divide the x and y values of the screen space positions by their w value
				screenPos = XMVectorDivide(screenPos, XMVectorSplatW(screenPos));
				screenPos2 = XMVectorDivide(screenPos2, XMVectorSplatW(screenPos2));

				// -1 ~ 1 ������ 0 ~ 1 �������� ��ȯ
				screenPos += {1.f, 1.f, 1.f, 1.f};
				screenPos2 += {1.f, 1.f, 1.f, 1.f};

				// y�� ��������
				screenPos = screenPos * 0.5f;
				screenPos.y = 1.f - screenPos.y;
				screenPos2 = screenPos2 * 0.5f;
				screenPos2.y = 1.f - screenPos2.y;

				// Multiply the x and y values by the size of the viewport in pixels
				int width, height;
				width = WindowManager::GetInstance()->GetClientWidth();
				height = WindowManager::GetInstance()->GetClientHeight();

				screenPos = XMVectorMultiply(screenPos, XMVectorSet(width, height, 0.0f, 0.0f));
				screenPos2 = XMVectorMultiply(screenPos2, XMVectorSet(width, height, 0.0f, 0.0f));

				GraphicsEngineSpace::ImGUIManager::GetInstance()->DrawLine({ screenPos.x, screenPos.y, screenPos.z }, { screenPos2.x, screenPos2.y, screenPos2.z });
				GraphicsEngineSpace::ImGUIManager::GetInstance()->DrawSpot({ screenPos.x, screenPos.y, screenPos.z });
			}
		}

		GraphicsEngineSpace::ImGUIManager::GetInstance()->End();
	}

	void NavMeshAgent::Update(float tick)
	{
		if (!NavigationManager::GetInstance()->navMesh || !isTrace)
			return;
		
		// ��ǥ�� �޶����ٸ� �ٲ��ش�. �ٵ� float �񱳿��� ��... ���� �����ΰͰ��⵵
		// �ƴϸ� �׳� Vector3.Distance(destination, currentDestination) < 0.1f �̷������� �ұ�..?
		if (!XMVector3Equal(destination, currentDestination))
		{
			currentDestination = destination;

			isChangeDestination = true;
		}

		if (isChangeDestination)
		{
			dtPolyRef startRef = 0;
			dtPolyRef endRef = 0;
			float startPos[3] = { transform->GetWorldPosition().x, transform->GetWorldPosition().y, transform->GetWorldPosition().z };
			float endPos[3] = { destination.x, destination.y, destination.z };

			Vector3 changeStartPos = ChangeCoordinate(startPos[0], startPos[1], startPos[2]);
			Vector3 changeEndPos = ChangeCoordinate(endPos[0], endPos[1], endPos[2]);

			startPos[0] = changeStartPos.x;
			startPos[1] = changeStartPos.y;
			startPos[2] = changeStartPos.z;
			endPos[0] = changeEndPos.x;
			endPos[1] = changeEndPos.y;
			endPos[2] = changeEndPos.z;

			// �־��� ���� ���� ����� navmesh �ٰ����� ã�µ� ���Ǵ� findNeaerstPoly�Լ�
			NavigationManager::GetInstance()->navQuery->findNearestPoly(startPos, polyPickExt, &NavigationManager::GetInstance()->filter, &startRef, 0);

			NavigationManager::GetInstance()->navQuery->findNearestPoly(endPos, polyPickExt, &NavigationManager::GetInstance()->filter, &endRef, 0);

			// start ��ġ�� poly�� end ��ġ�� poly�� ã�Ҵٸ�
			if (startRef && endRef)
			{
				// wayPoint �ʱ�ȭ
				wayPoint.clear();
				wayPointIndex = 0;

				// findPath : ���� ��ġ���� �� ��ġ���� navmesh�� ���� ��θ� ã�� Detour ���̺귯���� �Լ�
				// polys : ��θ� ��Ÿ���� ������ ������ ���ĵ� ��� / npolys : ��� �迭�� ��ȯ�� �ٰ��� �� / MAX_POLYS : ��� �迭�� ������ �� �ִ� �ִ� �ٰ��� ��
				NavigationManager::GetInstance()->navQuery->findPath(startRef, endRef, startPos, endPos, &NavigationManager::GetInstance()->filter, polys, &npolys, MAX_POLYS);
				nsmoothPath = 0;

				if (npolys)
				{
					// �� �޽� ǥ�鿡�� �ε巯�� ��θ� ã�� ���� ��θ� �ݺ��մϴ�.
					dtPolyRef _polys[MAX_POLYS];
					memcpy(_polys, polys, sizeof(dtPolyRef) * npolys);
					int _npolys = npolys;

					float iterPos[3], targetPos[3];
					NavigationManager::GetInstance()->navQuery->closestPointOnPoly(startRef, startPos, iterPos, 0);
					NavigationManager::GetInstance()->navQuery->closestPointOnPoly(_polys[_npolys - 1], endPos, targetPos, 0);

					static const float STEP_SIZE = 0.5f;
					static const float SLOP = 0.01f;

					nsmoothPath = 0;

					dtVcopy(&smoothPath[nsmoothPath * 3], iterPos);
					nsmoothPath++;

					// ��ǥ�� ������ ������ �� ���� ���ݾ� ��ǥ�� ���� �̵��մϴ�.
					while (_npolys && nsmoothPath < MAX_SMOOTH)
					{
						// ������ ��ġ�� ã���ϴ�.
						float steerPos[3];
						unsigned char steerPosFlag;
						dtPolyRef steerPosRef;

						if (!getSteerTarget(NavigationManager::GetInstance()->navQuery, iterPos, targetPos, SLOP,
							_polys, _npolys, steerPos, steerPosFlag, steerPosRef))
							break;

						bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
						bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

						// ������ ��Ÿ�� ã�´�.
						float delta[3], len;
						dtVsub(delta, steerPos, iterPos);
						len = dtMathSqrtf(dtVdot(delta, delta));
						// ���� ����� ����� ���̰ų� �޽� �ܺ� ��ũ�� ��� �ش� ��ġ�� ���� �̵����� �ʴ´�.
						if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
							len = 1;
						else
							len = STEP_SIZE / len;
						float moveTgt[3];
						dtVmad(moveTgt, iterPos, delta, len);

						// Move
						float result[3];
						dtPolyRef visited[16];
						int nvisited = 0;
						// ���� ��ġ���� �� ��ġ�� �̵��ϴ°Ű���
						NavigationManager::GetInstance()->navQuery->moveAlongSurface(_polys[0], iterPos, moveTgt, &NavigationManager::GetInstance()->filter,
							result, visited, &nvisited, 16);

						_npolys = fixupCorridor(_polys, _npolys, MAX_POLYS, visited, nvisited);
						_npolys = fixupShortcuts(_polys, _npolys, NavigationManager::GetInstance()->navQuery);

						float h = 0;
						NavigationManager::GetInstance()->navQuery->getPolyHeight(_polys[0], result, &h);
						result[1] = h;
						dtVcopy(iterPos, result);

						// ����� ��������� ��� ���� ���� �޽� ��ũ�� ó���մϴ�.
						if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
						{
							// Reached end of path.
							dtVcopy(iterPos, targetPos);
							if (nsmoothPath < MAX_SMOOTH)
							{
								dtVcopy(&smoothPath[nsmoothPath * 3], iterPos);
								nsmoothPath++;
							}
							break;
						}
						else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
						{
							// ���� �޽� ���ῡ �����߽��ϴ�.
							float startPos[3], endPos[3];

							// ���� �޽� ������� ��θ� ������ŵ�ϴ�.
							dtPolyRef prevRef = 0, polyRef = _polys[0];
							int npos = 0;
							while (npos < _npolys && polyRef != steerPosRef)
							{
								prevRef = polyRef;
								polyRef = _polys[npos];
								npos++;
							}
							for (int i = npos; i < _npolys; ++i)
								_polys[i - npos] = _polys[i];
							_npolys -= npos;

							// ������ ó���մϴ�.
							dtStatus status = NavigationManager::GetInstance()->navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
							if (dtStatusSucceed(status))
							{
								if (nsmoothPath < MAX_SMOOTH)
								{
									dtVcopy(&smoothPath[nsmoothPath * 3], startPos);
									nsmoothPath++;
									// ���� �޽� ���� �߿� ���� ��ΰ� ������ �ʵ��� ��ŷ?�մϴ�. ���Ҹ���
									if (nsmoothPath & 1)
									{
										dtVcopy(&smoothPath[nsmoothPath * 3], startPos);
										nsmoothPath++;
									}
								}
								// ���� �޽� ��ũ�� �ݴ������� ��ġ�� �̵��մϴ�.
								dtVcopy(iterPos, endPos);
								float eh = 0.0f;
								NavigationManager::GetInstance()->navQuery->getPolyHeight(_polys[0], iterPos, &eh);
								iterPos[1] = eh;
							}
						}

						// ����� �����մϴ�.
						if (nsmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&smoothPath[nsmoothPath * 3], iterPos);

							wayPoint.emplace_back(ChangeCoordinate(iterPos[0], iterPos[1], iterPos[2]));

							nsmoothPath++;
						}
					}
				}
				else
				{
					npolys = 0;
					nsmoothPath = 0;
				}

				// wayPoint�� �� �������� isChangeDestination false�ϰ� ��������
				isChangeDestination = false;
			}
		}

		if (!isChangeDestination)
		{
			// wayPoint�� �����ߴٸ� ����ó��
			if(wayPoint.empty())
				return;
			
			float distance = transform->GetWorldPosition().Distance(transform->GetWorldPosition(), wayPoint[wayPointIndex]);

			// ������� wayPoint�� ��������ٸ� ���� wayPoint�� ����
			if (distance <= 0.52f)
			{
				// ���m
				if (wayPointIndex >= wayPoint.size() - 1)
				{
					return;
				}
				else
				{
					wayPointIndex++;
				}
			}

			// ������ ���� �� normalize�� �̿��ؼ� ĳ���͸� wayPoint�� ��������
			/*SimpleMath::Vector3 moveDir = wayPoint[wayPointIndex] - transform->GetWorldPosition();
			moveDir.Normalize();

			transform->TransLate(moveDir * tick * speed);*/

			SimpleMath::Vector3 moveDir = wayPoint[wayPointIndex] - transform->GetWorldPosition();
			moveDir.Normalize();

			Vector3 velocity = moveDir;

			if (IsGround() && IsSlope())
			{
				// ���鿡 ������ ���͸� ����
				velocity = ProjectOnPlane(moveDir, { hitSlope.normal.vec[0], hitSlope.normal.vec[1], hitSlope.normal.vec[2] });

				velocity.Normalize();

				// �����̶�� gravity�� 0
				gravity = Vector3::Zero;

				// gravity �� ���ش�.
				rigidbody->SetGravity(false);

				rigidbody->SetLinearVelocity(velocity * speed + gravity);
			}
			else if (IsGround() && !IsSlope())
			{
				Vector3 down = { 0.f, -1.f, 0.f };

				gravity = Vector3::Down * abs(rigidbody->GetLinearVelocity().y);

				rigidbody->SetGravity(true);

				rigidbody->SetLinearVelocity(velocity * speed + gravity);
			}

			Vector3 rotateResult;
			rotateResult = Vector3::Lerp(transform->GetLook(), moveDir, tick * rotateSpeed);

			transform->SetLook({ rotateResult.x, 0.f, rotateResult.z });
		}
	}

	bool NavMeshAgent::IsSlope()
	{
		const std::vector<std::string> filterLayer = { "Default", "Player", "Enemy", "Wall", "Trigger" };
		Vector3 worldPos = transform->GetWorldPosition();

		// ĳ������ worldPos ���� �Ʒ��� 2.f ���̸�ŭ�� ray �� ���.
		hitSlope = PhysicsManager::GetInstance()->Raycast({ worldPos.x, worldPos.y, worldPos.z }, { 0, -1.0f, 0 }, 2.f, filterLayer, MoolyEngine::PhysicsType::INVALID);

		if (hitSlope.isHit)
		{
			SimpleMath::Vector3 normal = { hitSlope.normal.vec[0], hitSlope.normal.vec[1], hitSlope.normal.vec[2] };

			slopeAngle = XMVectorGetX(XMVector3AngleBetweenVectors(Vector3::Up, normal));

			slopeAngle = XMConvertToDegrees(slopeAngle);

			return slopeAngle != 0.f && slopeAngle < maxSlopeAngle;
		}

		return false;
	}

	bool NavMeshAgent::IsGround()
	{
		Vector3 worldPos = transform->GetWorldPosition();
		Vector3 scale = { 1.0f, 0.6f, 1.0f };

		const std::vector<std::string> filterLayer = { "Default", "Player", "Enemy", "Wall", "Trigger" };

		return PhysicsManager::GetInstance()->CheckBox({ worldPos.x, worldPos.y, worldPos.z }, { scale.x, scale.y, scale.z }, filterLayer);
	}

	SimpleMath::Vector3 NavMeshAgent::ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
	{
		float dot = vector.Dot(planeNormal);

		Vector3 scaledNormal = planeNormal * dot;

		return vector - scaledNormal;
	}

	void NavMeshAgent::Release()
	{
		__super::Release();
	}
}

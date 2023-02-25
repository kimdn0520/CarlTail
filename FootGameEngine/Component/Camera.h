#pragma once
#include "Object/ComponentBase.h"

//#include "Utils/ColliderUtils/BoundingVolumes.h"

namespace GraphicsEngineSpace
{
	class IDXObject;
}

namespace GameEngineSpace
{
	class Transform;

	// ���� Ÿ��
		// ����Ÿ�Կ� ���� View, Proj ���� �޶���.
	enum class ProjType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	/**
	 * \brief ������Ʈ�� ��ӹ��� ���ӽ����̽� ���� Camera
	 */
	class Camera final : public ComponentBase
	{
		// ��ġ ����
			// ��ǻ� ���⿡ ���� �ۼ��� ��ġ������ look up right ���Ͱ� ��� ����ִ�.
		std::shared_ptr<Transform> transform;

		// View, Proj ���
		Matrix view;
		Matrix proj;
		Matrix isoProj;

		ProjType projType;

		// ����ü�� ���õ� ������.
		float nearZ;
		float farZ;
		float aspect;
		float fovY;

		// ���Ŀ� ViewFrustum ���� ����, �Լ� �߰�.
			// ViewFrustum �ø�, ��ī�� �ڽ� ���� ��� �߰��� �ʿ�.
		// Frustum
		//std::shared_ptr<BoundingFrustum> viewFrustum;
		std::shared_ptr<GraphicsEngineSpace::IDXObject> skyBox;

		// Shaking factor
		float shakingIntensity;
		float originalZ;

	public:
		Camera(std::weak_ptr<GameObject> _obj);
		virtual ~Camera();

		virtual void Release() override;

		// ���� Getter Setter
		const float& GetAspectRatio() { return aspect; }
		// ȭ�� �� ����Ǹ� ProjMatrix�� ����Ǿ����.
		void SetAspectRatio(float _ratio) { aspect = _ratio; UpdateProjMatrix(projType); }
		const float& GetNearZ() { return nearZ; }
		void SetNearZ(float _nearZ) { nearZ = _nearZ; UpdateProjMatrix(projType); }
		const float& GetFarZ() { return farZ; }
		void SetFarZ(float _farZ) { farZ = _farZ; UpdateProjMatrix(projType); }
		const float& GetFov() { return fovY; }
		void SetFov(float _fov) { fovY = _fov; UpdateProjMatrix(projType); }

		// TM ��ȯ.
			// View�� Proj ����� ���� ���ϴ�.
			// Proj * View * SRT ������ View, Proj
		const Matrix& GetViewMatrix();
		const Matrix& GetProjMatrix();
		const Matrix& GetIsometricProjMatrix();

		virtual void Update(float tick) override;
		void Render();

		// �������� ����.
		void SetLens(float _fovY, float _aspect, float _nearZ, float _farZ);

		void SetShaking(float intensity);

		void UpdateViewMatrix();

		void UpdateProjMatrix(ProjType _projType);

		std::vector<SimpleMath::Vector3> ScreenToRayOrigin(const Vector2& mousePos);

		// ViewFrustumCulling�� ���� �Լ� ����
		//bool ViewFrustumCulling(std::shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix);
	
	private:
		void UpdateViewFrustum();
	};
}

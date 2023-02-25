#include "GamePch.h"
#include "Light.h"
#include "GraphicsManager.h"
#include "SceneBase.h"
#include "SceneManager.h"

#include "Object/GameObject.h"

namespace GameEngineSpace
{
	Light::Light(std::weak_ptr<GameObject> _gameObject) :
		ComponentBase(_gameObject, ComponentType::LIGHT)
		, lightType(LightType::Directional)
		, isShadow(true)
	{
		// Ʈ������ ĳ��
		transform = GetGameObject()->GetComponent<Transform>();
	}

	Light::~Light()
	{}

	void Light::Awake()
	{
		auto currentScene = SceneManager::GetInstance()->GetCurrentScene();

		if (currentScene == nullptr)
		{
			printf("NOT FOUND SCENE\n");
			return;
		}

		// Type�� ���� ���� �߰����ݴϴ�.
		switch (lightType)
		{
		case LightType::Point:
		{
			currentScene->AddPointLight(this->shared_from_this());
			break;
		}

		case LightType::Spot:
		{
			currentScene->AddSpotLight(this->shared_from_this());
			break;
		}

		case LightType::Directional:
		default:
			break;
		}
	}

	void Light::Release()
	{
		__super::Release();
	}

	// TODO : LightViewProj (�����츦 ����..)
	void Light::Update(float tick)
	{
		switch (lightType)
		{
		case LightType::Directional:
		{
			// view ����
			auto lightPos = -2.0f * 50.f * direction;

			if (lightPos != Vector3::Zero)
			{
				Matrix view = XMMatrixLookAtLH(lightPos, Vector3{ 0.5f, -0.5f, 0.5f }, Vector3{ 0.0f, 1.0f, 0.0f });

				Vector3 newCenter = XMVector3TransformCoord(Vector3{ 0.5f, -0.5f, 0.5f }, view);

				float l = newCenter.x - 50.f;
				float b = newCenter.y - 50.f;
				float n = newCenter.z - 50.f;
				float r = newCenter.x + 50.f;
				float t = newCenter.y + 50.f;
				float f = newCenter.z + 50.f;

				Matrix proj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

				lightViewProj = view * proj;
			}

			GraphicsManager::GetInstance()->PassDirectionalLight(color, direction, power, lightViewProj);
		}
		break;

		case LightType::Point:
		{
			position = transform->GetWorldPosition();

			// light view proj ���� ����
			std::vector<Matrix> lightViewProjMatrix(6);

			// �켱�� �ӽ� ���� ����
				// pointLight�� �������� ���� x, -x, y, -y, z, -z �������� ���ϴ� ����ü�� �׸��ٰ� �����ϸ� �ȴ�.
			if (position != Vector3::Zero)
			{
				// �ٶ󺸴� ������ lightPos �������� ��������.
				// X�� �ٶ󺸴� ���� + ������ UP�� ����.
				Matrix viewX = XMMatrixLookAtLH(position, position + Vector3{ 1.0f, 0.f, 0.f }, Vector3{ 0.f, 1.0f, 0.f });
				// -X�� �ٶ󺸴� ����
				Matrix viewXReci = XMMatrixLookAtLH(position, position + Vector3{ -1.0f, 0.f, 0.f }, Vector3{ 0.f, 1.0f, 0.f });
				// Z�� �ٶ󺸴� ����
				Matrix viewZ = XMMatrixLookAtLH(position, position + Vector3{ 0.0f, 0.f, 1.f }, Vector3{ 0.f, 1.0f, 0.f });
				// -Z�� �ٶ󺸴� ����
				Matrix viewZReci = XMMatrixLookAtLH(position, position + Vector3{ 0.0f, 0.f, -1.f }, Vector3{ 0.f, 1.0f, 0.f });
				// Y�� �ٶ󺸴� ���� + ���� UP�� ��� ������ ������ �����ؾ���.
				Matrix viewY = XMMatrixLookAtLH(position, position + Vector3{ 0.f, 1.f, 0.f }, Vector3{ 0.f, 0.f, -1.f });
				// -Y�� �ٶ󺸴� ����
				Matrix viewYReci = XMMatrixLookAtLH(position, position + Vector3{ 0.f, -1.f, 0.f }, Vector3{ 0.f, 0.f, 1.f });

				// projection�� ���, far�� range ��ŭ ������ ������ ������..?

				// ���̿� ���̰� ���ٰ� �����Ѵ�.
				float n = 1.f;
				float f = range;
				if (f < 1.0f)
					f = 1.0001f;
				// ������ �þ߰��� 90���� ���ݴϴ� => 90���� �������� (Pi / 2)
				Matrix proj = XMMatrixPerspectiveFovLH(0.5f * FootMath::Pi, 1.f, n, f);

				// ������ ���ؼ� vector�� �־��.
				// X
				lightViewProjMatrix[0] = viewX * proj;
				// -X
				lightViewProjMatrix[1] = viewXReci * proj;
				// Y
				lightViewProjMatrix[2] = viewY * proj;
				// -Y
				lightViewProjMatrix[3] = viewYReci * proj;
				// Z
				lightViewProjMatrix[4] = viewZ * proj;
				// -Z
				lightViewProjMatrix[5] = viewZReci * proj;
			}

			GraphicsManager::GetInstance()->PassPointLight(color, position, power, range, isShadow, lightViewProjMatrix);
		}
		break;

		case LightType::Spot:
		{
			position = transform->GetWorldPosition();

			// Spot Light�� Light View Proj�� ���, position�� ���� ���Ϳ� �ޱ��� ���� ����� �ݴϴ�.
			if (position != Vector3::Zero)
			{
				// �켱 View.. => �����ǿ��� Direction ������ ����..
				Vector3 focusDir = XMVector3Normalize(direction);

				// Direction�� GetLook�� ���� ���������� ������..
				// �ش� ������ Up Vector�� GetUp�� ���� ������ �� ���� ���̴�.
				Matrix view = XMMatrixLookAtLH(position, position + focusDir, transform->GetUp());

				// Perspective ������ ���� ����� ������ݴϴ�.
					// ���� ���̿� ���̰� ���ٰ� �������ݴϴ�. (������ �����Ǵ� ���� Ÿ���� �ƴ϶� ����)
				float n = 1.0f;
				float f = range;
				if (f < 1.f)
					f = 1.0001f;
				// �������� �ٲ��� ���� ����(����)
				float degToRad = 2.0f * FootMath::Pi * (outerSpotAngle / 360.f);
				if (degToRad == 0.f)
					degToRad = 0.0001f;
				Matrix proj = XMMatrixPerspectiveFovLH(degToRad, 1.0f, n, f);

				lightViewProj = view * proj;
			}

			GraphicsManager::GetInstance()->PassSpotLight(color, position, direction, power, innerSpotAngle, outerSpotAngle, range, isShadow, lightViewProj);
		}
		break;
		}
	}

	void Light::DebugIMGUIRender(float tick)
	{
		auto imguiManager = GraphicsEngineSpace::ImGUIManager::GetInstance();
		// ���⼭ IMGUI ���� (Frame)
		// �Ƹ� ���� ���⼭ �����ϱ� �������..?

		if (this->lightType == LightType::Directional)
		{
			if (imguiManager->TreeNode("Light In Scene", "DirLight" + std::to_string(GetObjectID())))
			{
				float LightColor[3] = { color.x, color.y, color.z };
				float LightDirection[3] = { direction.x, direction.y, direction.z };

				auto pos = transform->GetWorldPosition();

				float LightPos[3] = { pos.x, pos.y, pos.z };

				imguiManager->SliderFloat3("LightColor", LightColor, 0.f, 1.f);
				imguiManager->DragFloat("LightSpecular", &power, 0.f, 10.f);
				imguiManager->SliderFloat3("LightDir", LightDirection, -1.f, 1.f);
				imguiManager->DragFloat3("LightPos", LightPos);

				color.x = LightColor[0];
				color.y = LightColor[1];
				color.z = LightColor[2];

				direction.x = LightDirection[0];
				direction.y = LightDirection[1];
				direction.z = LightDirection[2];

				pos.x = LightPos[0];
				pos.y = LightPos[1];
				pos.z = LightPos[2];

				transform->SetPosition(pos);

				imguiManager->TreePop();
				imguiManager->Separator();
			}
		}

		if (this->lightType == LightType::Point)
		{
			if (imguiManager->TreeNode("Light In Scene", "PointLight" + std::to_string(GetObjectID())))
			{
				float LightColor[3] = { color.x, color.y, color.z };
				float LightDirection[3] = { direction.x, direction.y, direction.z };

				auto pos = transform->GetWorldPosition();

				float LightPos[3] = { pos.x, pos.y, pos.z };

				imguiManager->SliderFloat3("LightColor", LightColor, 0.f, 1.f);
				imguiManager->DragFloat("LightSpecular", &power);
				imguiManager->SliderFloat("LightRange", &range, 0.f, 100.f);
				imguiManager->SliderFloat3("LightDir", LightDirection, -1.f, 1.f);
				imguiManager->DragFloat3("LightPos", LightPos);

				color.x = LightColor[0];
				color.y = LightColor[1];
				color.z = LightColor[2];

				direction.x = LightDirection[0];
				direction.y = LightDirection[1];
				direction.z = LightDirection[2];

				pos.x = LightPos[0];
				pos.y = LightPos[1];
				pos.z = LightPos[2];

				transform->SetPosition(pos);

				imguiManager->TreePop();
				imguiManager->Separator();
			}
		}

		if (this->lightType == LightType::Spot)
		{
			if (imguiManager->TreeNode("Light In Scene", "SpotLight" + std::to_string(GetObjectID())))
			{
				float LightColor[3] = { color.x, color.y, color.z };
				float LightDirection[3] = { direction.x, direction.y, direction.z };

				auto pos = transform->GetWorldPosition();

				float LightPos[3] = { pos.x, pos.y, pos.z };

				imguiManager->SliderFloat3("LightColor", LightColor, 0.f, 1.f);
				imguiManager->DragFloat("LightSpecular", &power);
				imguiManager->SliderFloat("LightRange", &range, 0.f, 90.f);
				imguiManager->SliderFloat("LightInnerAngle", &innerSpotAngle, 0.f, outerSpotAngle);
				imguiManager->SliderFloat("LightOuterAngle", &outerSpotAngle, innerSpotAngle, 90.f);
				imguiManager->DragFloat3("LightDir", LightDirection, -1.f, 1.f);
				imguiManager->DragFloat3("LightPos", LightPos);

				color.x = LightColor[0];
				color.y = LightColor[1];
				color.z = LightColor[2];

				direction.x = LightDirection[0];
				direction.y = LightDirection[1];
				direction.z = LightDirection[2];

				pos.x = LightPos[0];
				pos.y = LightPos[1];
				pos.z = LightPos[2];

				transform->SetPosition(pos);

				imguiManager->TreePop();
				imguiManager->Separator();
			}
		}

	}
}
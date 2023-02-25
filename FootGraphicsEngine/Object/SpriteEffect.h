#pragma once
#include "Interface/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/// <summary>
	/// 3D ������ sprite Texture�� �޾Ƽ� ǥ������ Effect Object �Դϴ�.
	/// </summary>
	class SpriteEffect : public IDXObject
	{
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		std::vector<std::shared_ptr<MeshResources>> meshResources;

		// GetInstance�� ���̱� ���� ������ ����
		std::shared_ptr<ResourceManager> resourceManager;

		// TM ���
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// offSet���� ���� ���� float2 ����
		SimpleMath::Vector4 texOffset;

		// ���̴��� ������ ��Ʈ����
		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		// �����ڴ� Export�� ���־���� �Ѵ�.
		GRAPHICSENGINE_DECLSPEC SpriteEffect();
		~SpriteEffect() override;

		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) override;
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		virtual void PreRender(float tick) override {}
		virtual void Render() override;
		virtual void SetMeshResources(std::shared_ptr<MeshResources> objResources) override;
		virtual std::string GetObjName() override;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;

		virtual void SetTexOffset(float u, float v);
		virtual void SetTexOffset(const SimpleMath::Vector2& offset);
		virtual SimpleMath::Vector2 GetTexOffset();

		virtual const SimpleMath::Matrix& GetWorld() override { return world; }
		virtual const SimpleMath::Matrix& GetView() override { return view; }
		virtual const SimpleMath::Matrix& GetProj() override { return proj; }

	private:
		std::shared_ptr<Mesh> CreateEffectMesh(std::string name);
	};
}


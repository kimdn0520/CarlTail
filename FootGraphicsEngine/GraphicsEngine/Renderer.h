#pragma once

#include "Interface/IRenderer.h"

#include <vector>

using namespace std;
using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;
	class RenderTargetDepth;
	class RenderTargetTexture;
	class LightPass;

	/**
	 * \brief IRenderer�� ��ӹ޾� ������ ������ Ŭ����.
	 *
	 * ���� ������ �پ� �ִ� �κ��� ����߸��� ��� ��..
	 *
	 * 2022.06.14(ȭ) Dev.BlackFoot
	 */
	class Renderer : public IRenderer, public enable_shared_from_this<Renderer>
	{
		// ���� ���
		// ������ �ڵ�
		HWND hWnd;

		// Device, DC, RS ���� �� �ִ� �ھ� Ŭ����
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		// View���� ������ �ִ� ���� Ÿ�� Ŭ����
		std::shared_ptr<RenderTargetDepth> mainRenderTarget;

		// depth debug�� ���� Ÿ��.
		std::shared_ptr<RenderTargetTexture> DMRAORenderTarget;
		std::shared_ptr<RenderTargetTexture> normalRenderTarget;
		std::shared_ptr<RenderTargetTexture> albedoRenderTarget;
		std::shared_ptr<RenderTargetTexture> worldPosRenderTarget;
		std::shared_ptr<RenderTargetTexture> emissiveRenderTarget;

		// lightPass ����.
		std::shared_ptr<LightPass> lightPass;
		std::vector<std::shared_ptr<RenderTargetTexture>> gBuffer;

		// BlendState ����
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

		// �׷��� ����� ����. Ref by. ������ ��
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> annotation;
		unsigned int annotationCount;

		// Sprite
		std::shared_ptr<SpriteBatch> spriteBatch;

		float deltaTime;

		// ī�޶� ��ġ
		SimpleMath::Vector3 camPos;

		// â ������, ���� ��� ���õ� ���� ����.
			// �̰� ��������..?
		bool minimized;
		bool maximized;

		int clientWidth;
		int clientHeight;

	private:
		// ���丮�� �����
			// ȭ�鿡 �׷��� ������Ʈ ��ϵ�.
			// ���� ť? ���� ����?
		vector<std::shared_ptr<IDXObject>> renderVector;
	
	public:
		Renderer();
		virtual ~Renderer();

		virtual bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight) override;
		virtual bool LateInitialize() override;
		virtual void Finalize() override;
		virtual void OnResize() override;

		// ��Ⱦ�� ȹ��
		virtual float GetAspectRatio() const override;

		// â ������ ���� ����
		virtual bool IsVaildDevice() override;
		virtual void SetClientSize(int _width, int _height) override;
		virtual void SetWinMinMax(bool _isMinimized, bool _isMaximized) override;

		virtual void AddRenderObj(std::shared_ptr<IDXObject> obj) override;
		virtual void InitObject() override;
		virtual void InitObject(std::shared_ptr<IDXObject> obj) override;
		virtual void ClearRenderVector() override;

		virtual void BeginRender() override;
		virtual void Render() override;
		virtual void DebugRender() override;
		virtual void EndRender() override;
		// ĵ���� ����
		virtual std::shared_ptr<Canvas> CreateCanvas(const std::string& name, float width, float height) override;
		
		// ����� �̺�Ʈ
		virtual void GraphicsDebugBeginEvent(const std::string& name) override;
		virtual void GraphicsDebugEndEvent() override;

		virtual void SetCameraPos(float posX, float posY, float posZ) override;

		virtual void PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj) override;
		virtual void PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, SimpleMath::Matrix lightViewProj) override;
		virtual void PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, float halfAngle, float range, SimpleMath::Matrix lightViewProj) override;
	};
}

// ������ ��ü�� �����ϴ� �Լ��� ���� ���ش�.
extern "C" GRAPHICSENGINE_DECLSPEC GraphicsEngineSpace::IRenderer* CreateRenderer();
extern "C" GRAPHICSENGINE_DECLSPEC void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj);
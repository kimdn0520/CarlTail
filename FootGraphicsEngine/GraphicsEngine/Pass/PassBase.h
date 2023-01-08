#pragma once


namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;

	class RenderTargetDepth;

	/**
	 * \brief ���۵� �������� �⺻�� �Ǵ� Pass Rendering�� Base Class
	 */
	class PassBase
	{
	protected:
		static ComPtr<ID3D11Device> D3DDevice;
		static ComPtr<ID3D11DeviceContext> D3DDeviceContext;

	public:
		PassBase() = default;
		~PassBase() = default;

		static void Init();
		static void Reset();

	public:
		virtual void Start(const std::shared_ptr<RenderTargetDepth>& mainRTV) {}
		virtual void Release() abstract;
		virtual void OnResize(int width, int height) {}
	};
}
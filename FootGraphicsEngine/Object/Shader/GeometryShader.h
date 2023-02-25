#pragma once

#include "ShaderBase.h"

// ������Ʈ�� ���̴��� ���õ� ��ҵ��� ���ִ� Ŭ����
namespace GraphicsEngineSpace
{

	class GeometryShader : public ShaderBase
	{
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11GeometryShader> shader;

	public:
		GeometryShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob);
		~GeometryShader() override;

		bool SetUpShader() override;

		Shader* GetShader() override;

	private:
		bool CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob);
	};

}

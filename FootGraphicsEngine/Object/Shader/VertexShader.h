#pragma once
#include "ShaderBase.h"

// ���ؽ� ���̴��� ���õ� ��ҵ��� ���ִ� Ŭ����
namespace GraphicsEngineSpace
{
	class VertexShader : public ShaderBase
	{
		// ���ؽ����� inputLayout �ʿ�.
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11VertexShader> shader;
		ComPtr<ID3D11InputLayout> layout;

	public:
		VertexShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize);
		~VertexShader() override;

		bool SetUpShader() override;

		Shader* GetShader() override;

	private:
		// inputLayoutClass ���� ������ ����
		bool CreateLayout(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize);
		bool CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob);
	};
}
#pragma once
#include <map>
#include <string>

namespace GraphicsEngineSpace
{
	class ShaderBase;

	// �� Effect Ŭ���� ó�� ��� ���̴��� ������ �� ��, ������ �ִ� ���̴� �Ŵ���.
		// ����Ϸ��� ���̴��� ������ Ŭ������ �־���� �� ����.
	class ShaderManager
	{
	private:
		static std::shared_ptr<ShaderManager> instance;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3DBlob> shaderBlob;

		// ���̴� ������ �����ص� ��.
		std::map<std::string, std::shared_ptr<ShaderBase>> shaderMap;

	public:
		ShaderManager()
			: device(nullptr)
			, deviceContext(nullptr)
			, shaderBlob(nullptr)
		{}
		~ShaderManager();

		// �ν��Ͻ� ���
		static std::shared_ptr<ShaderManager> GetInstance();

		// ���� �ʱ�ȭ
		void Init(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext);

		// ��� ���̴� ������
		bool CompileAllShader();

		// ���ϴ� ���̴� ���
		std::shared_ptr<ShaderBase> GetShader(const std::string& shaderName);

		// �Ŵ��� ����
		void Finalize();

	private:
		// ���̴� ������ �ڵ�
		bool CompileShaderFromFile(const TCHAR* path, const char* entryName, const char* shaderModel, ID3DBlob** blobOut, const D3D_SHADER_MACRO* macro);

		bool CreateVertexShader(const std::string& path, const std::string& entryName, const std::string& shaderModel, const std::string& shaderName, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize, const
		                        D3D_SHADER_MACRO* macro);
		bool CreatePixelShader(const std::string& path, const std::string& entryName, const std::string& shaderModel, const std::string& shaderName, const
		                       D3D_SHADER_MACRO* macro);
	};

}
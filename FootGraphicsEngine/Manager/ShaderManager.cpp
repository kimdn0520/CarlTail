#include "GraphicsPch.h"
#include "ShaderManager.h"
#include "Object/Shader/VertexShader.h"
#include "Object/Shader/PixelShader.h"
#include "Object/Shader/GeometryShader.h"
#include "Resources/InputLayout.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ShaderManager> ShaderManager::instance = nullptr;

	ShaderManager::~ShaderManager()
	{
		if (shaderBlob != nullptr)
			shaderBlob->Release();
	}

	std::shared_ptr<ShaderManager> ShaderManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ShaderManager>();

		return instance;
	}

	void ShaderManager::Init(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext)
	{
		device = _device;
		deviceContext = _deviceContext;
	}

	bool ShaderManager::CompileAllShader()
	{
		// �ϴ��� �ϵ��ڵ�����
			// ���Ŀ� ������ ���� �� ������.. ���� �Լ��� �ϳ� �Ĵ� �͵� ������ ������.

		// shaderBlob�� ���̴� Ŭ������ ��ü�� ���� �� ���� �ٲ��־���Ѵ�.
			// �װ� ���⼭ ������ ������ִ� ������...
			// ���̴��� ���鶧 ���ø� �Լ��� �θ��� ������ �ϱ⵵ ������.. shaderManager�� �ʿ� �־��ֱ� ���ؼ� ���⼭ �غ��� ������
		// ���ؽ����̴� ����
		CreateVertexShader("Shader/LineShader/LineVS.hlsl", "main", "vs_5_0", "LineVS", InputLayoutDesc::Pos, 1, nullptr);
		CreateVertexShader("Shader/LineShader/BasicColorVS.hlsl", "main", "vs_5_0", "BasicColorVS", InputLayoutDesc::PosColor, 2, nullptr);
		CreateVertexShader("Shader/SkyBoxShader/SkyBoxVS.hlsl", "main", "vs_5_0", "SkyBoxVS", InputLayoutDesc::Pos, 1, nullptr);
		CreateVertexShader("Shader/SpriteEffectShader/SpriteEffectVS.hlsl", "main", "vs_5_0", "SpriteEffectVS", InputLayoutDesc::PosTex, 2, nullptr);

		// PBRShader
		D3D_SHADER_MACRO skinned_MacroVS[] = { {"Skinned"}, {NULL, NULL} };
		CreateVertexShader("Shader/PBRShader/PBRModelVS.hlsl", "main", "vs_5_0", "PBRStaticVS", InputLayoutDesc::PBRStatic, 5, nullptr);
		CreateVertexShader("Shader/PBRShader/PBRModelVS.hlsl", "main", "vs_5_0", "PBRSkinnedVS", InputLayoutDesc::PBRSkinned, 9, skinned_MacroVS);
		CreateVertexShader("Shader/Deferred/QuadVS.hlsl", "main", "vs_5_0", "QuadVS", InputLayoutDesc::PosTex, 2, nullptr);

		// Shadow
		D3D_SHADER_MACRO dir_MacroVS[] = { {"DirLightShadow"}, {NULL, NULL} };
		D3D_SHADER_MACRO skinned_Dir_MacroVS[] = { {"Skinned"}, {"DirLightShadow"}, {NULL, NULL} };
		CreateVertexShader("Shader/Deferred/ShadowVS.hlsl", "main", "vs_5_0", "ShadowStaticVS", InputLayoutDesc::PBRStatic, 5, dir_MacroVS);
		CreateVertexShader("Shader/Deferred/ShadowVS.hlsl", "main", "vs_5_0", "ShadowSkinnedVS", InputLayoutDesc::PBRSkinned, 9, skinned_Dir_MacroVS);
		// Geo�� ���� �͵��� PointLight shadow�� ���� ���ؽ� ���̴��̴�..
			// => Geometry ���̴����� �� �鿡 �ش��ϴ� LightViewProj�� �������ش�.
		CreateVertexShader("Shader/Deferred/ShadowVS.hlsl", "main", "vs_5_0", "GeoShadowStaticVS", InputLayoutDesc::PBRStatic, 5, nullptr);
		CreateVertexShader("Shader/Deferred/ShadowVS.hlsl", "main", "vs_5_0", "GeoShadowSkinnedVS", InputLayoutDesc::PBRSkinned, 9, skinned_MacroVS);

		// �׸��ڿ� ���ϼ��̴� ����
		CreateGeometryShader("Shader/Deferred/ShadowGS.hlsl", "main", "gs_5_0", "ShadowGS", nullptr);

		// �ȼ����̴� ����
		CreatePixelShader("Shader/LineShader/LinePS.hlsl", "main", "ps_5_0", "LinePS", nullptr);
		CreatePixelShader("Shader/LineShader/BasicColorPS.hlsl", "main", "ps_5_0", "BasicColorPS", nullptr);
		CreatePixelShader("Shader/SpriteEffectShader/SpriteEffectPS.hlsl", "main", "ps_5_0", "SpriteEffectPS", nullptr);

		// Macro
		D3D_SHADER_MACRO model_MacroPS_1[] = { {"USE_ALBEDO"}, {NULL, NULL} };
		D3D_SHADER_MACRO model_MacroPS_2[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {NULL, NULL} };
		D3D_SHADER_MACRO model_MacroPS_3[] = { {"USE_ALBEDO"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {NULL, NULL} };
		D3D_SHADER_MACRO model_MacroPS_4[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {NULL, NULL} };
		D3D_SHADER_MACRO model_MacroPS_5[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_AO"}, {NULL, NULL} };
		D3D_SHADER_MACRO model_MacroPS_6[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {"USE_AO"}, {NULL, NULL} };
		D3D_SHADER_MACRO model_MacroPS_7[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {"USE_AO"}, {"USE_EMISSIVE"}, {NULL, NULL} };

		// PBR Shader ����
		// basic
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRPS", nullptr);
		// albedo
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRAlbedoPS", model_MacroPS_1);
		// albedo + normal
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRNormalPS", model_MacroPS_2);
		// albedo + metallic + roughness
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRMetallicRoughnessPS", model_MacroPS_3);
		// albedo + normal + metallic + roughness
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRNormalMetallicRoughnessPS", model_MacroPS_4);
		// albedo + normal + ao
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRNormalAOPS", model_MacroPS_5);
		// albedo + normal + metallic + roughness + ao
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRNormalMRAOPS", model_MacroPS_6);
		// albedo + normal + metallic + roughness + ao + emissive
		CreatePixelShader("Shader/PBRShader/PBRModelPS.hlsl", "main", "ps_5_0", "PBRFullPS", model_MacroPS_7);

		D3D_SHADER_MACRO light_MacroPS_1[] = { {"IsShadow"}, {NULL, NULL} };
		D3D_SHADER_MACRO light_MacroPS_2[] = { { "IsPointShadow" }, { NULL, NULL } };
		D3D_SHADER_MACRO light_MacroPS_3[] = { { "IsSpotShadow" }, { NULL, NULL } };
		D3D_SHADER_MACRO light_MacroPS_4[] = { {"IsPointShadow"}, {"IsSpotShadow"}, {NULL, NULL}};

		// PBR Light
		CreatePixelShader("Shader/PBRShader/PBRLightPS.hlsl", "main", "ps_5_0", "PBRLightPS", nullptr);
		CreatePixelShader("Shader/PBRShader/PBRLightPS.hlsl", "main", "ps_5_0", "PBRLightShadowPS", light_MacroPS_1);
		CreatePixelShader("Shader/PBRShader/PBRLightPS.hlsl", "main", "ps_5_0", "PBRLightPointShadowPS", light_MacroPS_2);
		CreatePixelShader("Shader/PBRShader/PBRLightPS.hlsl", "main", "ps_5_0", "PBRLightSpotShadowPS", light_MacroPS_3);
		CreatePixelShader("Shader/PBRShader/PBRLightPS.hlsl", "main", "ps_5_0", "PBRLightPointSpotShadowPS", light_MacroPS_4);

		// PostPorcessing
		D3D_SHADER_MACRO post_Macro_ToneMap[] = {{"UseToneMapping"}, {NULL, NULL}};
		D3D_SHADER_MACRO post_Macro_All[] = {{"UseToneMapping"}, {"UseVignette"}, {NULL, NULL}};
		CreatePixelShader("Shader/Deferred/PostProcessPS.hlsl", "main", "ps_5_0", "PostToneMappingPS", post_Macro_ToneMap);
		CreatePixelShader("Shader/Deferred/PostProcessPS.hlsl", "main", "ps_5_0", "PostProcessAllPS", post_Macro_All);

		// SkyBox
		CreatePixelShader("Shader/SkyBoxShader/SkyBoxPS.hlsl", "main", "ps_5_0", "SkyBoxPS", nullptr);

		return true;
	}

	std::shared_ptr<ShaderBase> ShaderManager::GetShader(const std::string& shaderName)
	{
		if (shaderMap.find(shaderName) == shaderMap.end())
			return nullptr;

		return shaderMap.at(shaderName);
	}

	void ShaderManager::Finalize()
	{
		// ���� ��ȸ �ϸ鼭 ��� �ڵ� ����
		for(auto shader : shaderMap)
		{
			shader.second.reset();
		}

		shaderMap.clear();

		SafeReset(instance)
	}

	/**
	 * \brief hlsl�ϳ��� �������ϴ� �Լ�
	 * \param path ���̴� ������ path
	 * \param entryName shader �ڵ� ������ entryPointer name
	 * \param shaderModel ����� ���̴� ���α׷��� ������ ��� ����.
	 * \param blobOut ���̴��� �������ؼ� ������ blob�� ������
	 * \return ���� ����
	 */
	bool ShaderManager::CompileShaderFromFile(const TCHAR* path, const char* entryName, const char* shaderModel,
	                                          ID3DBlob** blobOut, const D3D_SHADER_MACRO* macro)
	{
		if (path == nullptr || entryName == nullptr || shaderModel == nullptr || blobOut == nullptr)
			return false;

		*blobOut = nullptr;

		UINT flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(
			path,
			macro,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryName,
			shaderModel,
			flags,
			0,
			blobOut,
			&errorBlob);

		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
				errorBlob->Release();
			}

			return false;
		}

		if (errorBlob != nullptr)
			errorBlob->Release();

		return true;
	}

	bool ShaderManager::CreateVertexShader(const std::string& path, const std::string& entryName,
	                                       const std::string& shaderModel, const std::string& shaderName, D3D11_INPUT_ELEMENT_DESC* desc,
	                                       UINT elementsSize, const D3D_SHADER_MACRO* macro)
	{
		// �ʿ� �ش� �̸��� ���̴��� ������ �������ݴϴ�
			// �ٽ� ���� �ʿ� ���� �����Դϴ�.
		if(shaderMap.find(shaderName) != shaderMap.end())
			return false;

		std::shared_ptr<ShaderBase> tmpShader;

		// Ȥ�� �� ��� ����
		if (shaderBlob != nullptr)
		{
			//shaderBlob->Release();
			shaderBlob = nullptr;
		}

		// ���� ���ڸ� ����մϴ�.
		std::wstring wpath;

		wpath.assign(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob, macro) != true)
			return false;

		// �߰��� �ش� ���ؽ� ���̴��� ����ϴ� ��ǲ ���̾ƿ��� ������ش�.
		tmpShader.reset(new VertexShader(device, deviceContext, shaderBlob, desc, elementsSize));

		if (tmpShader == nullptr)
		{
			assert(0);		// ���� ó��
		}

		// ������� ���ؽ� ���̴��� �ʿ� �־��ش�.
		shaderMap.insert(std::pair{shaderName, tmpShader });

		printf("%s Compile Complete! \n", shaderName.c_str());

		// ������� ���� ����
		return true;
	}

	bool ShaderManager::CreatePixelShader(const std::string& path, const std::string& entryName,
	                                      const std::string& shaderModel, const std::string& shaderName, const D3D_SHADER_MACRO* macro)
	{
		// �ʿ� �ش� �̸��� ���̴��� ������ �������ݴϴ�
			// �ٽ� ���� �ʿ� ���� �����Դϴ�.
		if (shaderMap.find(shaderName) != shaderMap.end())
			return false;

		// �⺻������ ���� �Ͱ� ����ϳ� pixelshader�� �����մϴ�.
		std::shared_ptr<ShaderBase> tmpShader;

		// Ȥ�� �� ��� ����
		if (shaderBlob != nullptr)
		{
			//shaderBlob->Release();
			shaderBlob = nullptr;
		}

		// ���� ���ڸ� ����մϴ�.
		std::wstring wpath = std::wstring(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob, macro) != true)
			return false;

		tmpShader.reset(new PixelShader(device, deviceContext, shaderBlob));

		if (tmpShader == nullptr)
		{
			assert(0);		// ���� ó��
		}

		// ������� �ȼ� ���̴��� �ʿ� �־��ش�.
		shaderMap.insert(std::pair{ shaderName, tmpShader });

		printf("%s Compile Complete! \n", shaderName.c_str());


		// ������� ���� ����
		return true;
	}

	bool ShaderManager::CreateGeometryShader(const std::string& path, const std::string& entryName,
		const std::string& shaderModel, const std::string& shaderName, const D3D_SHADER_MACRO* macro)
	{
		// �ʿ� �ش� �̸��� ���̴��� ������ �������ݴϴ�
			// �ٽ� ���� �ʿ� ���� �����Դϴ�.
		if (shaderMap.find(shaderName) != shaderMap.end())
			return false;
			
		std::shared_ptr<ShaderBase> tmpShader;

		// Ȥ�� �� ��� ����
		if (shaderBlob != nullptr)
		{
			shaderBlob = nullptr;
		}

		// ���� ���ڸ� ����մϴ�.
		std::wstring wpath = std::wstring(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob, macro) != true)
			return false;

		tmpShader.reset(new GeometryShader(device, deviceContext, shaderBlob));

		if (tmpShader == nullptr)
		{
			assert(0);		// ���� ó��
		}

		// ������� ������Ʈ�� ���̴��� �ʿ� �־��ش�.
		shaderMap.insert(std::pair{ shaderName, tmpShader });

		printf("%s Compile Complete! \n", shaderName.c_str());

		// ������� ���� ����
		return true;
	}
}

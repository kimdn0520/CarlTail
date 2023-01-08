#include "GraphicsPch.h"
#include "UIUtilsManager.h"
#include "ResourceManager.h"

using namespace ATL;

namespace GraphicsEngineSpace
{
	std::shared_ptr<UIUtilsManager> UIUtilsManager::instance = nullptr;


	UIUtilsManager::~UIUtilsManager()
	{
		
	}

	std::shared_ptr<UIUtilsManager> UIUtilsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<UIUtilsManager>();

		return instance;
	}

	void UIUtilsManager::Init(ComPtr<ID3D11Device> _device, std::shared_ptr<SpriteBatch> spriteBatch, ComPtr<ID3D11DepthStencilState> depthState, ComPtr
	                          <ID3D11BlendState> blendState)
	{
		device = _device;
		this->spriteBatch = spriteBatch;
		this->depthState = depthState;
		this->blendState = blendState;

		// ���⼭ ��� ��Ʈ�� ������ݴϴ�.
		CreateSpriteFont("gulim", "Font/gulim9k.spritefont");
		CreateSpriteFont("EBS-Light", "Font/ebsLight.spritefont");

		// Lim Texture�� �����ؼ� ID�� ������ �ְ� ����ϴ�.
		limTex = ResourceManager::GetInstance()->LoadTexture(L"Resources/UI/UILim.png");
	}

	std::shared_ptr<SpriteFont> UIUtilsManager::GetFont(const std::string& fontName)
	{
		if(fontMap.find(fontName) == fontMap.end())
			return nullptr;

		return fontMap.at(fontName);
	}

	void UIUtilsManager::Finalize()
	{
		for(auto font : fontMap)
		{
			font.second.reset();
		}

		fontMap.clear();

		SafeReset(spriteBatch)

		SafeReset(instance);
	}

	void UIUtilsManager::CreateSpriteFont(const std::string& name, const std::string& path)
	{
		TCHAR buffer[256] = {};

		_tcscpy_s(buffer, CA2T(path.c_str()));

		std::shared_ptr<SpriteFont> dxFont = std::make_shared<SpriteFont>(device.Get(), buffer);
		dxFont->SetLineSpacing(32.0f);

		// ������� ��Ʈ �ʿ� �־��ֱ�
		fontMap.insert(std::pair{name, dxFont});
	}
}

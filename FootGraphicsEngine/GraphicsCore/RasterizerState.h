#pragma once

namespace GraphicsEngineSpace
{

	/**
	* \brief ���� ���� �� �� ���� RaterizerState�� �̾Ƶ� Ŭ����.
	*
	* ������ Effectó�� RS���� ���⼭ �̾��ش�.
	*
	* 2022.07.12(ȭ) Dev.BlackFoot
	*/
	class RasterizerState
	{
	private:
		static ComPtr<ID3D11RasterizerState> wireFrameRS;
		static ComPtr<ID3D11RasterizerState> solidRS;
		static ComPtr<ID3D11RasterizerState> pointDepthRS;
		static ComPtr<ID3D11RasterizerState> spotDepthRS;

	public:
		static bool InitAllRS(ComPtr<ID3D11Device> pDevice);

		static ComPtr<ID3D11RasterizerState> GetPointDepthRS() { return pointDepthRS; }
		static ComPtr<ID3D11RasterizerState> GetSpotDepthRS() { return spotDepthRS; }

		static ComPtr<ID3D11RasterizerState> GetWireFrameRS() { return wireFrameRS; }
		static ComPtr<ID3D11RasterizerState> GetSolidRS() { return solidRS; }
		static void DestroyAll();
	};

}

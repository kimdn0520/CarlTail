#pragma once

namespace GraphicsEngineSpace
{
	/// �ҿ� ������ InputLayoutŬ������ �����ؼ� ������ inputlayout Ŭ����.
	/// ���������� ���ؽ� ����ü�� �ٸ� ������Ʈ���� inputlayout�� ������ ������,
	///	�ʿ��� �� �����ϴ� ������ �Ѵ�. => Effect Ŭ������ ����� ����..
	///
	///	�⺻������ �� ������ object�� �پ��ִ� BuildVertexLayout�Լ� �κ��� Ŭ������ �̾��ذ�.
	///	2022.07.06(��) Dev.BlackFoot

	/**
	 * \brief ���ؽ� ����ü�� �������� ���� ��ǲ���̾ƿ� Desc�� ������ش�.
	 */
	class InputLayoutDesc
	{
	public:
		// ���� �⺻���� InputLayout ��ġ�� �÷� ����.
		static D3D11_INPUT_ELEMENT_DESC PosColor[2];

		// Effect ǥ���� ���� ��ġ�� UV����
		static D3D11_INPUT_ELEMENT_DESC PosTex[2];
		
		// PBR
		static D3D11_INPUT_ELEMENT_DESC PBRStatic[5];

		static D3D11_INPUT_ELEMENT_DESC PBRSkinned[9];

		// ��ī�� �ڽ� DESC
		static D3D11_INPUT_ELEMENT_DESC Pos[1];
	};

}
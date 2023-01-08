#pragma once

/// ���� ���̴����� ������ �־���ϴ� ��ҵ��� wrap�ص� shaderBase Ŭ����
///	shader manager���� Map�� ������ �ִ� ���

#include <string>

namespace GraphicsEngineSpace
{
	// ���̴� ���� enum
	enum class ShaderType
	{
		NONE,
		VERTEX,
		PIXEL
	};

	// ���̴� ����ü => ������ ������ ��.
		// ������ �ּҸ� �����ϴ� �ǹ̿� �����
	struct Shader
	{
		int unused;
	};

	class ShaderBase
	{
	protected:
		ShaderType type;

	public:
		ShaderBase(ShaderType type)
			: type(type) {}
		virtual  ~ShaderBase() = default;

		// SetUp �� Get
		// SetUp DC�� ���̴��� �������ִ� �ܰ�
		virtual bool SetUpShader() abstract;
		virtual Shader* GetShader() abstract;
	};
}

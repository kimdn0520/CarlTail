#pragma once

#include "Object/Shader/ShaderBase.h"

namespace GraphicsEngineSpace
{
	// ���ؽ��� �ε��� ���۵� ���⼭ ������ ���� �ִ�..�� �ϴ��� constant��..
	enum class BufferType
	{
		NONE,
		CONSTANT
	};

	class BufferBase
	{
		BufferType type;

	public:
		BufferBase(BufferType type)
			: type()
		{}

		virtual ~BufferBase() = default;

		virtual bool SetUpBuffer(unsigned int slot, void* data, ShaderType targetType) abstract;
	};

}
#include "GraphicsPch.h"
#include "ConstantBuffer.h"

namespace GraphicsEngineSpace
{
	ConstantBuffer::ConstantBuffer(ComPtr<ID3D11DeviceContext> deviceContext)
		: BufferBase(BufferType::CONSTANT)
		, deviceContext(deviceContext)
		, buffer(nullptr)
		, bufferSize(0)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
		
	}

	bool ConstantBuffer::SetUpBuffer(unsigned slot, void* data, ShaderType targetType)
	{
		if (buffer == nullptr)
			return false;

		// �� ���̴��� �°� ���۸� �������ݴϴ�.
		// SetupBuffer ���� ���۰� ������� �־���մϴ�..!
			// GPU�� � �ڿ��� ������Ʈ �� �� �ʿ��� �Լ�.
		// ���ⵥ���Ϳ��� ���̴��� ���ͼ� �����ִ� ����ΰ�..
		// ���̴� �����͸� ���ͼ� ���̵� �����ͷ� �ٲ㼭 �� �Լ��� �����ָ� �Լ� ������ �Ͻ��� ĳ������ �Ͼ�� ��?
		deviceContext->UpdateSubresource(buffer.Get(), 0, nullptr, data, bufferSize, 0);

		switch (targetType)
		{
			case ShaderType::VERTEX:
			{
				// �� ���Կ� ���� �ϳ���..
					// �� ���̴��� constBuffer�� �������̸� �������� ���۸� ���� ���� �� �ִ�.
					// �׷��� �ߺ��Ǵ� ���� �����Ϸ��� �̷��� �Ѱǰ�
				deviceContext->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());

				return true;
			}

			case ShaderType::PIXEL:
			{
				deviceContext->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());

				return true;
			}
		}

		return false;
	}

	bool ConstantBuffer::CreateBuffer(ComPtr<ID3D11Device> device, UINT typeSize)
	{
		// �������� ���������� BufferBuilder�� Friend Ű���带 ���� �ű⼭ ������ִ� ���ϴ�.
		D3D11_BUFFER_DESC bufferDescription;
		// ���� ���� ������ֱ�
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));

		// ���ۿ� ���� �־��ֱ�
			// ��ǻ� UpdateSubresource�� ����� ���̱� ������ ���⼭ ��������.
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.ByteWidth = typeSize;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = 0;

		bufferSize = typeSize;

		HRESULT hr = device->CreateBuffer(&bufferDescription, nullptr, buffer.GetAddressOf());

		return SUCCEEDED(hr);
	}
}

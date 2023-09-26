#include <vector>

#include "MeshModel.h"
#include "D3D11Utils.h"


void MeshModel::Initialize(ComPtr<ID3D11Device>& device, MeshData& mesh)
{
	// Constant Buffer ����
	m_constantBufferData.world = Matrix(); // World ���
	m_constantBufferData.view = Matrix(); // View ���
	m_constantBufferData.proj = Matrix(); // Projection ���
	D3D11Utils::CreateConstantBuffer(device, m_constantBufferData, m_meshVSConstantBuffer);

	// Shader ����

	// InputLayer�� ������ �������� Vertex Shader�� ���� ������ ����ü�� ���� ����
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		// COLOR�� ���۵Ǵ� ��ġ�� POSITION�� RGB ���� 4 byte�� �Ҵ��� �������� �����ϹǷ� 4(byte) * 3(����)
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 9, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// InputLayout & Vertex Shader ����
	D3D11Utils::CreateVSAndInputLayout(device, L"Shader/BasicVS.hlsl", inputElements,
		m_meshVertexShader, m_meshInputLayout);

	// Pixel Shader ����
	D3D11Utils::CreatePS(device, L"Shader/BasicPS.hlsl", m_meshPixelShader);
}

void MeshModel::Render(ComPtr<ID3D11DeviceContext>* context)
{
}

void MeshModel::UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context)
{
}

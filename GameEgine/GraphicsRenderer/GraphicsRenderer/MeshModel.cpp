#include <vector>

#include "MeshModel.h"
#include "D3D11Utils.h"


void MeshModel::Initialize(ComPtr<ID3D11Device>& device, MeshData& mesh)
{
	// Constant Buffer 생성
	m_constantBufferData.world = Matrix(); // World 행렬
	m_constantBufferData.view = Matrix(); // View 행렬
	m_constantBufferData.proj = Matrix(); // Projection 행렬
	D3D11Utils::CreateConstantBuffer(device, m_constantBufferData, m_meshVSConstantBuffer);

	// Shader 생성

	// InputLayer의 데이터 형식으로 Vertex Shader에 들어가는 데이터 구조체와 같게 설정
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		// COLOR가 시작되는 위치로 POSITION이 RGB 각각 4 byte씩 할당한 다음부터 시작하므로 4(byte) * 3(개수)
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 9, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// InputLayout & Vertex Shader 생성
	D3D11Utils::CreateVSAndInputLayout(device, L"Shader/BasicVS.hlsl", inputElements,
		m_meshVertexShader, m_meshInputLayout);

	// Pixel Shader 생성
	D3D11Utils::CreatePS(device, L"Shader/BasicPS.hlsl", m_meshPixelShader);
}

void MeshModel::Render(ComPtr<ID3D11DeviceContext>* context)
{
}

void MeshModel::UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context)
{
}

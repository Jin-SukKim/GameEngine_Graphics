#include <vector>

#include "MeshModel.h"
#include "D3D11Utils.h"


void MeshModel::Initialize(ComPtr<ID3D11Device>& device, MeshData& mesh)
{
	// VertexBuffer ���� �� CPU -> GPU ������ ����
	D3D11Utils::CreateVertexBuffer(device, mesh.vertices, m_mesh.vertexBuffer);
	// Index Buffer ���� �� CPU -> GPU ������ ����
	D3D11Utils::CreateIndexBuffer(device, mesh.indices, m_mesh.indexBuffer);
	// Index Count ����
	m_mesh.indexCount = (UINT)mesh.indices.size();

	// Constant Buffer ����
	m_constantVSBufferData.world = Matrix(); // World ���
	m_constantVSBufferData.view = Matrix(); // View ���
	m_constantVSBufferData.proj = Matrix(); // Projection ���
	D3D11Utils::CreateConstantBuffer(device, m_constantVSBufferData, m_meshVSConstantBuffer);

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

void MeshModel::Render(ComPtr<ID3D11DeviceContext>& context)
{
	// Graphics Pipeline�� �ܰ�
	// 1. �׸����� �ϴ� ���� Vertex�� Index ����
	// 2. IA: Input-Assembler stage (�Է�(Vertex�� Index��)�� ��Ƽ� �������� �� �⺻ ����(�ﰢ�� ��)�� �����)
	//          (ex: vertex 3���� ��� �ﰢ�� �����)
	// 3. VS: Vertex Shader - �� ��ȯ, ���� ��ȯ, Projection
	// 4. Tessellation
	// 5. Geometry shader : 4, 5���� ������ ���� �� �ڼ��ϰ� ����� �ܰ��̴�.
	// 6. RS: Rasterizer stage - ���� ���� ������ pixel�� �������� �����.
	// 7. PS: Pixel Shader - pixel���� ���� ����
	// 8. OM: Output-Merger stage - depth buffering ���� �� ���� FrameBuffer�� ���� ���� �����Ѵ�.

	// Vertex Shader ����
	context->VSSetShader(m_meshVertexShader.Get(), 0, 0);
	// Pixel Shader ����
	context->PSSetShader(m_meshPixelShader.Get(), 0, 0);

	/*
		// ���� constant buffer ����
		ID3D11Buffer *pptr[1] = {
			m_constantBuffer.Get(),
		};
		m_context->VSSetConstantBuffers(0, 1, pptr);
	*/
	// Constant Buffer ����
	// (0�� index���� ����, 1��, constant buffer)
	context->VSSetConstantBuffers(0, 1, m_meshVSConstantBuffer.GetAddressOf());

	// Vertex Buffer�� ������ offset ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Input Layout ����
	context->IASetInputLayout(m_meshInputLayout.Get());
	// Vertex/Index Buffer ����
	context->IASetVertexBuffers(0, 1, m_mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_mesh.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Index Buffer�� ���� Vertex���� ������� ���� (_TRIANGLESTRIP ��)
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // index 3���� ��� �ﰢ�� �����

	// GPU�� �غ�Ǹ� Render
	// (�� ���� �׸��� ����, Buffer���� �� ���� index�κ��� �׸��� ������ �� ����)
	context->DrawIndexed(m_mesh.indexCount, 0, 0);

}

void MeshModel::UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context)
{
	// Constant Buffer Data�� CPU -> GPU ����
	D3D11Utils::UpdateBuffer(context, m_constantVSBufferData, m_meshVSConstantBuffer);
}
#include <vector>

#include "MeshModel.h"
#include "D3D11Utils.h"
#include "GeometryGenerator.h"


void MeshModel::Initialize(ComPtr<ID3D11Device>& device, const std::vector<MeshData>& meshes)
{
	// Texture Sampler ����
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	// Filtering�� Linear Interpolation���� ����
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// Texture ��� �ۿ� �ִ� �ؽ��� ��ǥ Ȯ�� ���
	// wrap/crap/mirror �� �ɼ� ���� ����
	// Texture�� 2D Texture, 3D Textrue�� �ֱ� ������ W ��ǥ���� ��� ����
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // x
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // y
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // z
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	// LOD(Level Of Detail) - �ַ� �Ÿ� ������� ���
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �ϳ��� ���� ���� ���� �޽��� �̷���� ������ Sampler�� Constant buffer��
	// �ϳ��� ����ؾ� ���� �޽����� �ϳ��� ��ó�� ���δ�.
	// Sampler State ����
	device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

	// Constant Buffer ����
	m_constantVSBufferData.world = Matrix(); // world ���
	//m_constantVSBufferData.view = Matrix(); // View ���
	//m_constantVSBufferData.proj = Matrix(); // Projection ���
	m_constantVSBufferData.viewProj = Matrix(); // viewPorjection ���
	D3D11Utils::CreateConstantBuffer(device, m_constantVSBufferData, m_meshVSConstantBuffer);
	D3D11Utils::CreateConstantBuffer(device, m_constantPSBufferData, m_meshPSConstantBuffer);

	for (const MeshData& mData : meshes)
	{
		std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>();
		// VertexBuffer ���� �� CPU -> GPU ������ ����
		D3D11Utils::CreateVertexBuffer(device, mData.vertices, newMesh->vertexBuffer);
		// Index Buffer ���� �� CPU -> GPU ������ ����
		D3D11Utils::CreateIndexBuffer(device, mData.indices, newMesh->indexBuffer);
		// Index Count ����
		newMesh->indexCount = (UINT)mData.indices.size();

		
		// Texture ����
		if (!mData.texturePath.empty()) {
			D3D11Utils::CreateTexture(device, mData.texturePath,
				newMesh->meshTexture, newMesh->txtResView);
		}

		// �ϳ��� ���̹Ƿ� ���� constant data ���
		newMesh->constantBufferVS = m_meshVSConstantBuffer;
		newMesh->constantBufferPS = m_meshPSConstantBuffer;

		m_meshes.push_back(newMesh);
	}

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

	// Normal Vector �׸��� - ������ ������ InputLayout�� ���� ���
	m_normal = std::make_shared<Mesh>();

	std::vector<Vertex> normalVertices;
	std::vector<uint32_t> normalIndices;

	// ���� �޽��� normal���� �ϳ��� ��ġ��
	size_t offset = 0;
	for (const MeshData& mData : meshes) {
		// ������ Mesh�� data ���
		for (size_t i = 0; i < mData.vertices.size(); i++)
		{
			Vertex v = mData.vertices[i];

			// �������� ������ ��ǥ�� ������ texcoord�� ���� 0.f, 1.f�� �ٸ��⿡ vertex shader���� ��ȯ
			// Normal Vector�� �����̱⿡ texcoord�� x��ǥ�� ���
			// ������
			v.texcoord.x = 0.f;
			normalVertices.push_back(v);

			// ����
			v.texcoord.x = 1.f;
			normalVertices.push_back(v);

			// 0-1 ����, 2-3 ���� etc
			normalIndices.push_back(uint32_t(2 * (i + offset)));
			normalIndices.push_back(uint32_t(2 * (i + offset) + 1));
		}
		// 1d array�� 2d array�� ����ϴ� ��ó�� ���� �޽����� normal��
		// offset�� Ȱ���� �ϳ��� �����ش�.
		offset += mData.vertices.size();
	}

	// Normal Line�� vertex�� index buffer ����
	D3D11Utils::CreateVertexBuffer(device, normalVertices, m_normal->vertexBuffer);
	m_normal->indexCount = (UINT)normalIndices.size();
	D3D11Utils::CreateIndexBuffer(device, normalIndices, m_normal->indexBuffer);

	D3D11Utils::CreateConstantBuffer(device, m_constantNormalBufferData, m_meshNormalConstantBuffer);

	// Normal Vector Line�� Shader ����
	D3D11Utils::CreateVSAndInputLayout(
		device, L"Shader/NormalVS.hlsl", inputElements, m_meshNormalVertexShader, m_meshInputLayout);
	D3D11Utils::CreatePS(device, L"Shader/NormalPS.hlsl", m_meshNormalPixelShader);
}

void MeshModel::Initialize(ComPtr<ID3D11Device>& device, const std::string& basePath, const std::string& fileName)
{
	// ���Ϸκ��� ������ ��������
	std::vector<MeshData> meshes = GeometryGenerator::ReadFromFile(basePath, fileName);
	// ������ �����ͷ� buffer �ʱ�ȭ
	Initialize(device, meshes);
}

void MeshModel::Render(ComPtr<ID3D11DeviceContext>& context, CubeMap& cubeMap)
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
	// Texture�� �� Sampler �ѱ��
	context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf()); 


	// Vertex Buffer�� ������ offset ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (const std::shared_ptr<Mesh>& mesh : m_meshes)
	{
		/*
			// ���� constant buffer ����
			ID3D11Buffer *pptr[1] = {
				m_constantBuffer.Get(),
			};
			m_context->VSSetConstantBuffers(0, 1, pptr);
		*/
		// Constant Buffer ����
		// (0�� index���� ����, 1��, constant buffer)
		context->VSSetConstantBuffers(0, 1, mesh->constantBufferVS.GetAddressOf());

		// Pixel Shader�� Texture�� Sampler�� �Ѱ��ش�.
		// Texture Data�� txtResView�� Shader���� ����ϴ� Resource��
		// ������ �����Ͱ� �� �ִ�.
		// ����Ƽ�� ���� Texture�� ��� ���� Texture�� �Բ� ����ϴ� ��찡 ���� �迭�� ����� �ѱ��.
		ComPtr<ID3D11ShaderResourceView> pixelResources[4] =
		{
			mesh->txtResView.Get(),
			cubeMap.GetTextureResView().Get(),
			cubeMap.GetDiffuseResView().Get(),
			cubeMap.GetSpecularResView().Get(),
		};
		// ���� ResourceView�� ����� Texture�� 2���� 2�� ����
		context->PSSetShaderResources(0, 4, pixelResources->GetAddressOf()); // txtResView �ѱ��
		context->PSSetConstantBuffers(0, 1, mesh->constantBufferPS.GetAddressOf());

		// Input Layout ����
		context->IASetInputLayout(m_meshInputLayout.Get());
		// Vertex/Index Buffer ����
		context->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Index Buffer�� ���� Vertex���� ������� ���� (_TRIANGLESTRIP ��)
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // index 3���� ��� �ﰢ�� �����
		// GPU�� �غ�Ǹ� Render
		// (�� ���� �׸��� ����, Buffer���� �� ���� index�κ��� �׸��� ������ �� ����)
		context->DrawIndexed(mesh->indexCount, 0, 0);
	}

	// Normal Vector �׸���
	if (drawNormal) {
		context->VSSetShader(m_meshNormalVertexShader.Get(), 0, 0);
		context->PSSetShader(m_meshNormalPixelShader.Get(), 0, 0);

		// Normal Vector�� Shader������ Mesh�� vertex constant buffer�� data�� ���
		ComPtr<ID3D11Buffer> pptr[2] = {
			m_meshVSConstantBuffer.Get(),
			m_meshNormalConstantBuffer.Get()
		};

		// 0�� index�� �����ϰ� 2���� buffer�� ���
		context->VSSetConstantBuffers(0, 2, pptr->GetAddressOf());
		
		context->IASetVertexBuffers(0, 1, m_normal->vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(m_normal->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	
		// ���� �׸���
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->DrawIndexed(m_normal->indexCount, 0, 0);
	}

}

void MeshModel::UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context)
{
	// Constant Buffer Data�� CPU -> GPU ����
	D3D11Utils::UpdateBuffer(context, m_constantVSBufferData, m_meshVSConstantBuffer);
	D3D11Utils::UpdateBuffer(context, m_constantPSBufferData, m_meshPSConstantBuffer);
	D3D11Utils::UpdateBuffer(context, m_constantNormalBufferData, m_meshNormalConstantBuffer);
}

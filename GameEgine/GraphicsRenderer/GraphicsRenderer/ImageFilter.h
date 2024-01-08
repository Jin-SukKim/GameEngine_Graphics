#pragma once
#include "D3D11Utils.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "string.h"

class ImageFilter
{
public:
	ImageFilter(ComPtr<ID3D11Device>& device,
		ComPtr<ID3D11DeviceContext>& context,
		const std::wstring& vertexPrefix, const std::wstring& pixelPrefix,
		int width, int height) {
		Initialize(device, context, vertexPrefix, vertexPrefix, width, height);
	}

	void Initialize(ComPtr<ID3D11Device>& device,
		ComPtr<ID3D11DeviceContext>& context,
		const std::wstring& vertexPrefix, const std::wstring& pixelPrefix,
		int width, int height);

	void UpdateConstantBuffers(ComPtr<ID3D11Device>& context);

	// �̹��� ������ �Է�
	void SetShaderResources(
		const std::vector<ComPtr<ID3D11ShaderResourceView>>& resources)
	{
		m_shaderResources.clear();
		for (const auto& res : resources)
			m_shaderResources.push_back(res.Get());
	}

	// ��� ����� �� ����
	void SetRenderTargets(
		const std::vector<ComPtr<ID3D11RenderTargetView>>& targets) {
		m_renderTargets.clear();
		for (const auto& target : targets)
			m_renderTargets.push_back(target.Get());
	}
public:
	struct SamplingPixelConstData {
		// �ȼ����� ����
		float dx;
		float dy;
		float threshold; // Bloom Filter������ ��� �Ѱ�ġ
		float strength; // filter ����
		DirectX::SimpleMath::Vector4 option;
	};
	SamplingPixelConstData m_pixelConstData;
private:

	std::shared_ptr<Mesh> m_mesh;

	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11SamplerState> m_samplerState;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;
	
	// ȭ��(�ַ� ������) ��� �������� ��
	D3D11_VIEWPORT m_viewPort;
	
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// �޾Ƽ� ����ϱ⿡ �����ͷ� ���
	// ����� texture���� view
	std::vector<ID3D11ShaderResourceView*> m_shaderResources; 
	// ��� �������� ��
	std::vector<ID3D11RenderTargetView*> m_renderTargets;
};


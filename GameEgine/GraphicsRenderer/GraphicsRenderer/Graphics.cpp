#include "Graphics.h"

auto MakeBox()
{
    std::vector<Vector3> positions;
    std::vector<Vector3> colors;
    std::vector<Vector3> normals;

    const float scale = 1.0f;

    // ����
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

    // �Ʒ���
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));

    // �ո�
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

    // �޸�
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

    // ����
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));

    // ������
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));

    // Vertex ������ ������ �ִ�.
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.color = colors[i];
        vertices.push_back(v);
    }

    // index�� ���� unsigned int16���� �ް� �ִ�. (16bit)
    // ū ���� ����� ���� uint32_t�� ����ϰ� ������ uint8_t�� ����Ѵ�.
    std::vector<uint16_t> indices = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
    };

    // tuple�� ����� �� vector�� return
    return std::tuple{ vertices, indices };

}

// ������ü ����
Graphics::Graphics() : AppBase(), m_indexCount(0)
{
}

bool Graphics::Initialize() 
{
	if (!AppBase::Initialize())
		return false;

    // Geometry ����
    auto [vertices, indices] = MakeBox();

    // Vertex Buffer ���� �� CPU -> GPU ������ ����
    AppBase::CreateVertexBuffer(vertices, m_vertexBuffer);

    // Index Buffer ���� �� CPU -> GPU ������ ����
    m_indexCount = UINT(indices.size());
    AppBase::CreateIndexBuffer(indices, m_indexBuffer);

    // ��ȯ ������ �� ����� Buffer
    m_constantBufferData.world = Matrix();      // World ���
    m_constantBufferData.view = Matrix();       // View ���
    m_constantBufferData.proj = Matrix(); // Projection ���
    AppBase::CreateConstantBuffer(m_constantBufferData, m_constantBuffer);

    // Shader ����
    
    // InputLayout�� ������ �������� Vertex�� ���� ������ ����ü�� ���� ����
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        // COLOR�� ���۵Ǵ� ��ġ�� POSITION�� RGB ���� 4 byte�� �Ҵ��� �������� �����ϹǷ� 4(byte) * 3(����)
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    // InputLayout & Vertex Shader ����
    AppBase::CreateVSAndInputLayout(L"Shader/BasicVS.hlsl", inputElements,
        m_vertexShader, m_inputLayout);

    // Pixel Shader ����
    AppBase::CreatePS(L"Shader/BasicPS.hlsl", m_pixelShader);

	return true;
}

// CPU���� ��ü�� ��� �������� ��� ��� �� GPU�� ����
void Graphics::Update(float dt)
{
    static float rot = 0.f;
    rot += dt;
    // ���� ��ȯ ���
    m_constantBufferData.world = Matrix::CreateScale(0.5f)
        * Matrix::CreateRotationY(rot) 
        * Matrix::CreateTranslation(Vector3(0.f, 0.f, 1.0f));
    // DirectX�� Row-Major ����ϳ� HLSL���� Shader ���α׷��� Column-Major ���
    m_constantBufferData.world = m_constantBufferData.world.Transpose(); // Row-Major -> Column-Major ��ȯ

    // ���� ��ȯ - ���� = ������ �����̴� �ݴ�� ������ ��� ���� �����δ�.
    m_constantBufferData.view =
        // (ī�޶� ��ġ, ���� ����, ī�޶��� upVector)
        DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
    m_constantBufferData.view = m_constantBufferData.view.Transpose();

    // ���� ���
    const float aspect = AppBase::GetAspectRatio();
    if (m_usePerspectiveProjection)
    {
        // �þ߰�
        const float fovAngleY = 70.f * DirectX::XM_PI / 180.f;
        // ���� ����
        m_constantBufferData.proj =
            // (�þ߰�, ȭ�����, Near-Plane, Far-Plane)
            DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.f);
    }
    else
    {
        // ������
        m_constantBufferData.proj =
            // (x ���� ���� ���� 2��, y ���� ���� ���� 2��, Near-Plane, Far-Plane)
            DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
    }
    m_constantBufferData.proj = m_constantBufferData.proj.Transpose();

    // Constant Buffer Data�� CPU -> GPU ����
    AppBase::UpdateBuffer(m_constantBufferData, m_constantBuffer);
}

void Graphics::Render()
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

    // ȭ�鿡 ����� view port ����
    m_context->RSSetViewports(1, &m_screenViewPort);

    float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
    // �� frame �׸��� �� ���� ������ �ʱ�ȭ
    m_context->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);
    // Depth/Stencil ���� �ʱ�ȭ
    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    // RenderTarget ����
    m_context->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), m_depthStencilView.Get());
    
    // Depth/Stencil State ����
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0); // �⺻ state

    // Vertex Shader ����
    m_context->VSSetShader(m_vertexShader.Get(), 0, 0);

    /*
        // ���� constant buffer ���� 
        ID3D11Buffer *pptr[1] = {
            m_constantBuffer.Get(),
        };
        m_context->VSSetConstantBuffers(0, 1, pptr);
    */ 
    // Vertex Shader�� Constant Buffer ����
    // (0�� index���� ����, 1��, constant buffer)
    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    // Pixel Shader ����
    m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
    
    // Rasterizer State ����
    m_context->RSSetState(m_rasterizerState.Get());

    // Vertex/Index Buffer ����
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    
    // Input Layout ����
    m_context->IASetInputLayout(m_inputLayout.Get());
    // Vertex/Index Buffer ����
    m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Index Buffer�� ���� Vertex���� ������� ����
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 3���� ��� �ﰢ��
    
    // GPU�� �غ�Ǹ� Render ����
    // (�� ���� �׸��� ����, Buffer���� �� ���� index�κ��� �׸��� ������ �� ����)
    m_context->DrawIndexed(m_indexCount, 0, 0);
}

void Graphics::UpdateGUI()
{
    ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
}
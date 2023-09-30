#include "Graphics.h"
#include "GeometryGenerator.h"
Graphics::Graphics() : AppBase()
{
}

bool Graphics::Initialize() 
{
	if (!AppBase::Initialize())
		return false;

    // Circle ����
    {
        // Geometry ����
        MeshData sphere = GeometryGenerator::MakeSphere(1.5f, 5, 5);
        // subdivision
        sphere = GeometryGenerator::SubdivideToSphere(1.5f, sphere);
        m_mesh.Initialize(m_device, sphere);
    }

	return true;
}

// CPU���� ��ü�� ��� �������� ��� ��� �� GPU�� ����
void Graphics::Update(float dt)
{
    // ���� ��ȯ ���
    Matrix world = Matrix::CreateScale(m_scale)
        * Matrix::CreateRotationX(m_rotationX)
        * Matrix::CreateRotationY(m_rotationY)
        * Matrix::CreateRotationZ(m_rotationZ)
        * Matrix::CreateTranslation(Vector3(m_tranlationX, m_tranlationY, m_tranlationZ));
    // DirectX�� Row-Major ����ϳ� HLSL���� Shader ���α׷��� Column-Major ���
    m_mesh.m_constantVSBufferData.world = world.Transpose(); // Row-Major -> Column-Major ��ȯ

    // ���� ��ȯ - ���� = ������ �����̴� �ݴ�� ������ ��� ���� �����δ�.
    Matrix view =
        // (ī�޶� ��ġ, ���� ����, ī�޶��� upVector)
        DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
    m_mesh.m_constantVSBufferData.view = view.Transpose();

    Matrix proj;
    // ���� ���
    const float aspect = AppBase::GetAspectRatio();
    if (m_usePerspectiveProjection)
    {
        // �þ߰�
        const float fovAngleY = 70.f * DirectX::XM_PI / 180.f;
        // ���� ����
        proj =
            // (�þ߰�, ȭ�����, Near-Plane, Far-Plane)
            DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.f);
    }
    else
    {
        // ������
        proj =
            // (x ���� ���� ���� 2��, y ���� ���� ���� 2��, Near-Plane, Far-Plane)
            DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
    }
    m_mesh.m_constantVSBufferData.proj = proj.Transpose();

    m_mesh.UpdateConstantBuffers(m_context);
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
    
    // Rasterizer State ����
    if (m_wireFrame) // 
        m_context->RSSetState(m_WireRasterizerState.Get());
    else
        m_context->RSSetState(m_SolidRasterizerState.Get());
    
    m_mesh.Render(m_context);
}

void Graphics::UpdateGUI()
{
    ImGui::Checkbox("WireFrame", &m_wireFrame);
    ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);

    ImGui::SliderFloat("Scale", &m_scale, -50.f, 50.f);
    
    ImGui::SliderFloat("Traslation X", &m_tranlationX, -50.f, 50.f);
    ImGui::SliderFloat("Traslation Y", &m_tranlationY, -50.f, 50.f);
    ImGui::SliderFloat("Traslation Z", &m_tranlationZ, -50.f, 50.f);
    
    ImGui::SliderFloat("Rotation X", &m_rotationX, -50.f, 50.f);
    ImGui::SliderFloat("Rotation Y", &m_rotationY, -50.f, 50.f);
    ImGui::SliderFloat("Rotation Z", &m_rotationZ, -50.f, 50.f);

}
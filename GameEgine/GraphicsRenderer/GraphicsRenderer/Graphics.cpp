#include "Graphics.h"
#include "GeometryGenerator.h"

#include <directxtk/SimpleMath.h>

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
        //MeshData sphere = GeometryGenerator::MakeSphere(1.5f, 5, 5);
        MeshData sphere = GeometryGenerator::MakeCube();
        // subdivision
        //sphere = GeometryGenerator::SubdivideToSphere(1.5f, sphere);
        m_mesh.Initialize(m_device, sphere);
    }

	return true;
}

// CPU���� ��ü�� ��� �������� ��� ��� �� GPU�� ����
void Graphics::Update(float dt)
{
    using DirectX::SimpleMath::Quaternion;
    // ���� ��ȯ ���
    Quaternion q = Quaternion::CreateFromYawPitchRoll(Vector3(m_rotationX, m_rotationY, m_rotationZ));
    Matrix world = Matrix::CreateScale(m_scale)
        * Matrix::CreateFromQuaternion(q)
        * Matrix::CreateTranslation(Vector3(m_tranlationX, m_tranlationY, m_tranlationZ));
    // DirectX�� Row-Major ����ϳ� HLSL���� Shader ���α׷��� Column-Major ���
    m_mesh.m_constantVSBufferData.world = world.Transpose(); // Row-Major -> Column-Major ��ȯ

    // ī�޶��� �̵�
    UserInput(dt);

    //Matrix view = m_camera.GetFocusViewRowMatrix();
    Matrix view = m_camera.GetFPPViewRowMatrix();
    m_mesh.m_constantVSBufferData.view = view.Transpose();

    Matrix proj = m_camera.GetProjRowMatrix();
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

    ImGui::SliderFloat("Scale", &m_scale, -10.f, 10.f);
    
    ImGui::SliderFloat("Traslation X", &m_tranlationX, -5.f, 5.f);
    ImGui::SliderFloat("Traslation Y", &m_tranlationY, -5.f, 5.f);
    ImGui::SliderFloat("Traslation Z", &m_tranlationZ, -5.f, 5.f);
    
    ImGui::SliderFloat("Rotation X", &m_rotationX, -3.14f, 3.14f);
    ImGui::SliderFloat("Rotation Y", &m_rotationY, -3.14f, 3.14f);
    ImGui::SliderFloat("Rotation Z", &m_rotationZ, -3.14f, 3.14f);
}

void Graphics::UserInput(float dt)
{
    // ī�޶��� �̵�
    // �ð� ���ݿ� ����ؼ� �̵� (dt ���)
    if (m_keyPressed[87]) // w : ������ �̵�
        m_camera.MoveForward(dt);
    if (m_keyPressed[83]) // s : ������ �̵�
        m_camera.MoveForward(-dt);
    if (m_keyPressed[68]) // d : ���������� �̵�
        m_camera.MoveRight(dt);
    if (m_keyPressed[65]) // a : �������� �̵�
        m_camera.MoveRight(-dt);
    if (m_keyPressed[69]) // d : ���� �̵�
        m_camera.MoveUp(dt);
    if (m_keyPressed[81]) // a : �Ʒ��� �̵�
        m_camera.MoveUp(-dt);


}

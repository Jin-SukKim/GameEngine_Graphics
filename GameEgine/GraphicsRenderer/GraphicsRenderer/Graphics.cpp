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

    // �⺻ �� ����
    {
        // Geometry ����
        //MeshData square = GeometryGenerator::MakeGrid(2.0f, 1.7f, 100, 70);
        //MeshData square = GeometryGenerator::MakeSphere(1.5f, 15, 15);
        //square.texturePath = "../Assets/Textures/blender_uv_grid_2k.png";
        //m_mesh.Initialize(m_device, { square });
        m_mesh.Initialize(
            m_device, 
            "C:/Study/Project/GameEgine/GraphicsRenderer/Assets/Models/f3d-data/zelda-breath-of-the-wild/source/zeldaPosed001/",
            "zeldaPosed001.fbx");
    }

	return true;
}

// CPU���� ��ü�� ��� �������� ��� ��� �� GPU�� ����
void Graphics::Update(float dt)
{
    using DirectX::SimpleMath::Quaternion;
    // ���� ��ȯ ���
    Quaternion q = Quaternion::CreateFromYawPitchRoll(m_rotation);
    Matrix world = Matrix::CreateScale(m_scale)
        * Matrix::CreateFromQuaternion(q)
        * Matrix::CreateTranslation(m_translation);
    // DirectX�� Row-Major ����ϳ� HLSL���� Shader ���α׷��� Column-Major ���
    m_mesh.m_constantVSBufferData.world = world.Transpose(); // Row-Major -> Column-Major ��ȯ

    m_mesh.m_constantVSBufferData.invWorld = m_mesh.m_constantVSBufferData.world;
    // ���� ����
    m_mesh.m_constantVSBufferData.invWorld.Translation(Vector3(0.f));
    m_mesh.m_constantVSBufferData.invWorld = m_mesh.m_constantVSBufferData.invWorld.Transpose().Invert();

    // ī�޶��� �̵�
    UserInput(dt);

    //Matrix view = m_camera.GetFocusViewRowMatrix();
    Matrix view = m_camera.GetFPPViewRowMatrix();
    m_mesh.m_constantVSBufferData.view = view.Transpose();

    Matrix proj = m_camera.GetProjRowMatrix();
    m_mesh.m_constantVSBufferData.proj = proj.Transpose();

    m_mesh.m_constantPSBufferData.camWorld = m_camera.GetCameraPos();
    m_mesh.m_constantPSBufferData.useTexture = m_useTexture;
    m_mesh.m_constantPSBufferData.material.shininess = m_shininess;
    m_mesh.m_constantPSBufferData.material.diffuse = Vector3(m_diffuse);
    m_mesh.m_constantPSBufferData.material.specular = Vector3(m_specular);

    // �������� ������ ����� ��
    for (int i = 0; i < MAX_LIGHTS; i++) {
        // �ٸ� ���� ����
        if (i != lightType) {
            m_mesh.m_constantPSBufferData.lights[i].strength *= 0.0f;
        }
        else {
            m_mesh.m_constantPSBufferData.lights[i] = m_light;
        }
    }

    if (m_normalLine)
    {   
        m_mesh.m_constantNormalBufferData.scale = m_normalScale;
    }

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
    
    m_mesh.Render(m_context, m_normalLine);
}

void Graphics::UpdateGUI()
{
    ImGui::Checkbox("WireFrame", &m_wireFrame);
    ImGui::Checkbox("Use PerspectiveProjection", &m_usePerspectiveProjection);
    ImGui::Checkbox("Use Texture", &m_useTexture);
    ImGui::Checkbox("Draw Normal Vector", &m_normalLine);

    ImGui::SliderFloat3("Scale", &m_scale.x, 0.f, 10.f);
    
    ImGui::SliderFloat3("Model Translation", &m_translation.x, -3.14f, 3.14f);
    ImGui::SliderFloat3("Model Rotation", &m_rotation.x, -3.14f, 3.14f);

    ImGui::SliderFloat("Material Shininess", &m_shininess, 0.f, 256.f);
    ImGui::SliderFloat("Material Diffuse Color", &m_diffuse, 0.f, 1.f);
    ImGui::SliderFloat("Material Specular Color", &m_specular, 0.f, 1.f);

    ImGui::SliderFloat("Normal Vector Scale", &m_normalScale, 0.0f, 1.f);

    if (ImGui::RadioButton("Directional Light", lightType == 0)) {
        lightType = 0;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Point Light", lightType == 1)) {
        lightType = 1;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Spot Light", lightType == 2)) {
        lightType = 2;
    }

    ImGui::SliderFloat3("Light Position", &m_light.pos.x, -5.f, 5.f);
    ImGui::SliderFloat3("Light Strength", &m_light.strength.x, 0.f, 1.f);
    ImGui::SliderFloat("Light Spot Power", &m_light.spotPower, 1.f, 512.f);
    ImGui::SliderFloat("Light FallOffStart", &m_light.fallOffStart, 0.f, 100.f);
    ImGui::SliderFloat("Light FallOffEnd", &m_light.fallOffEnd, 0.f, 100.f);
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

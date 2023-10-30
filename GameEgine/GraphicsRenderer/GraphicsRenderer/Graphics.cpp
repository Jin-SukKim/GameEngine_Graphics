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

    // 기본 모델 생성
    {
        // Geometry 정의
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

// CPU에서 물체가 어떻게 움직일지 행렬 계산 후 GPU로 전송
void Graphics::Update(float dt)
{
    using DirectX::SimpleMath::Quaternion;
    // 모델의 변환 행렬
    Quaternion q = Quaternion::CreateFromYawPitchRoll(m_rotation);
    Matrix world = Matrix::CreateScale(m_scale)
        * Matrix::CreateFromQuaternion(q)
        * Matrix::CreateTranslation(m_translation);
    // DirectX는 Row-Major 사용하나 HLSL같은 Shader 프로그램은 Column-Major 사용
    m_mesh.m_constantVSBufferData.world = world.Transpose(); // Row-Major -> Column-Major 변환

    m_mesh.m_constantVSBufferData.invWorld = m_mesh.m_constantVSBufferData.world;
    // 오류 방지
    m_mesh.m_constantVSBufferData.invWorld.Translation(Vector3(0.f));
    m_mesh.m_constantVSBufferData.invWorld = m_mesh.m_constantVSBufferData.invWorld.Transpose().Invert();

    // 카메라의 이동
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

    // 여러개의 조명을 사용할 때
    for (int i = 0; i < MAX_LIGHTS; i++) {
        // 다른 조명 끄기
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
    // Graphics Pipeline의 단계
    // 1. 그리고자 하는 모델의 Vertex와 Index 버퍼
    // 2. IA: Input-Assembler stage (입력(Vertex와 Index등)을 모아서 렌더링을 할 기본 단위(삼각형 등)을 만든다)
    //          (ex: vertex 3개를 모아 삼각형 만들기)
    // 3. VS: Vertex Shader - 모델 변환, 시점 변환, Projection
    // 4. Tessellation
    // 5. Geometry shader : 4, 5번은 생성한 모델을 더 자세하게 만드는 단계이다.
    // 6. RS: Rasterizer stage - 모델의 기하 정보를 pixel의 집합으로 만든다.
    // 7. PS: Pixel Shader - pixel들을 색을 결정
    // 8. OM: Output-Merger stage - depth buffering 적용 등 실제 FrameBuffer의 색깔 값을 결정한다.

    // 화면에 출력할 view port 설정
    m_context->RSSetViewports(1, &m_screenViewPort);

    float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
    // 새 frame 그리기 전 이전 데이터 초기화
    m_context->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);
    // Depth/Stencil 버퍼 초기화
    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    // RenderTarget 설정
    m_context->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), m_depthStencilView.Get());
    
    // Depth/Stencil State 설정
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0); // 기본 state
    
    // Rasterizer State 설정
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
    // 카메라의 이동
    // 시간 간격에 비례해서 이동 (dt 사용)
    if (m_keyPressed[87]) // w : 앞으로 이동
        m_camera.MoveForward(dt);
    if (m_keyPressed[83]) // s : 뒤으로 이동
        m_camera.MoveForward(-dt);
    if (m_keyPressed[68]) // d : 오른쪽으로 이동
        m_camera.MoveRight(dt);
    if (m_keyPressed[65]) // a : 왼쪽으로 이동
        m_camera.MoveRight(-dt);
    if (m_keyPressed[69]) // d : 위로 이동
        m_camera.MoveUp(dt);
    if (m_keyPressed[81]) // a : 아래로 이동
        m_camera.MoveUp(-dt);


}

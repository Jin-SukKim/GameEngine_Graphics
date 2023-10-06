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

    // Circle 생성
    {
        // Geometry 정의
        //MeshData sphere = GeometryGenerator::MakeSphere(1.5f, 5, 5);
        MeshData sphere = GeometryGenerator::MakeCube();
        // subdivision
        //sphere = GeometryGenerator::SubdivideToSphere(1.5f, sphere);
        m_mesh.Initialize(m_device, sphere);
    }

	return true;
}

// CPU에서 물체가 어떻게 움직일지 행렬 계산 후 GPU로 전송
void Graphics::Update(float dt)
{
    using DirectX::SimpleMath::Quaternion;
    // 모델의 변환 행렬
    Quaternion q = Quaternion::CreateFromYawPitchRoll(Vector3(m_rotationX, m_rotationY, m_rotationZ));
    Matrix world = Matrix::CreateScale(m_scale)
        * Matrix::CreateFromQuaternion(q)
        * Matrix::CreateTranslation(Vector3(m_tranlationX, m_tranlationY, m_tranlationZ));
    // DirectX는 Row-Major 사용하나 HLSL같은 Shader 프로그램은 Column-Major 사용
    m_mesh.m_constantVSBufferData.world = world.Transpose(); // Row-Major -> Column-Major 변환

    // 카메라의 이동
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

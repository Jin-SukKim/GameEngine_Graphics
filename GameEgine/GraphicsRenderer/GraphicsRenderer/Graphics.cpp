#include "Graphics.h"
#include "GeometryGenerator.h"
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
        MeshData sphere = GeometryGenerator::MakeSphere(1.5f, 5, 5);
        // subdivision
        sphere = GeometryGenerator::SubdivideToSphere(1.5f, sphere);
        m_mesh.Initialize(m_device, sphere);
    }

	return true;
}

// CPU에서 물체가 어떻게 움직일지 행렬 계산 후 GPU로 전송
void Graphics::Update(float dt)
{
    // 모델의 변환 행렬
    Matrix world = Matrix::CreateScale(m_scale)
        * Matrix::CreateRotationX(m_rotationX)
        * Matrix::CreateRotationY(m_rotationY)
        * Matrix::CreateRotationZ(m_rotationZ)
        * Matrix::CreateTranslation(Vector3(m_tranlationX, m_tranlationY, m_tranlationZ));
    // DirectX는 Row-Major 사용하나 HLSL같은 Shader 프로그램은 Column-Major 사용
    m_mesh.m_constantVSBufferData.world = world.Transpose(); // Row-Major -> Column-Major 변환

    // 시점 변환 - 원리 = 시점이 움직이는 반대로 세상의 모든 모델을 움직인다.
    Matrix view =
        // (카메라 위치, 보는 방향, 카메라의 upVector)
        DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
    m_mesh.m_constantVSBufferData.view = view.Transpose();

    Matrix proj;
    // 투영 행렬
    const float aspect = AppBase::GetAspectRatio();
    if (m_usePerspectiveProjection)
    {
        // 시야각
        const float fovAngleY = 70.f * DirectX::XM_PI / 180.f;
        // 원근 투영
        proj =
            // (시야각, 화면비율, Near-Plane, Far-Plane)
            DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.f);
    }
    else
    {
        // 정투영
        proj =
            // (x 방향 범위 변수 2개, y 방향 범위 변수 2개, Near-Plane, Far-Plane)
            DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
    }
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

    ImGui::SliderFloat("Scale", &m_scale, -50.f, 50.f);
    
    ImGui::SliderFloat("Traslation X", &m_tranlationX, -50.f, 50.f);
    ImGui::SliderFloat("Traslation Y", &m_tranlationY, -50.f, 50.f);
    ImGui::SliderFloat("Traslation Z", &m_tranlationZ, -50.f, 50.f);
    
    ImGui::SliderFloat("Rotation X", &m_rotationX, -50.f, 50.f);
    ImGui::SliderFloat("Rotation Y", &m_rotationY, -50.f, 50.f);
    ImGui::SliderFloat("Rotation Z", &m_rotationZ, -50.f, 50.f);

}
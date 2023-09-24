#include "Graphics.h"
#include "GeometryGenerator.h"
Graphics::Graphics() : AppBase(), m_indexCount(0)
{
}

bool Graphics::Initialize() 
{
	if (!AppBase::Initialize())
		return false;

    // Circle 생성
    {
        // Geometry 정의
        MeshData mesh = GeometryGenerator::MakeGrid(2.0f, 2.0f, 10, 10);

        // Vertex Buffer 생성 후 CPU -> GPU 데이터 복사
        D3D11Utils::CreateVertexBuffer(m_device, mesh.vertices, m_vertexBuffer);

        // Index Buffer 생성 후 CPU -> GPU 데이터 복사
        m_indexCount = UINT(mesh.indices.size());
        D3D11Utils::CreateIndexBuffer(m_device, mesh.indices, m_indexBuffer);
    }

    // 변환 정의할 때 사용할 Buffer
    m_constantBufferData.world = Matrix();      // World 행렬
    m_constantBufferData.view = Matrix();       // View 행렬
    m_constantBufferData.proj = Matrix(); // Projection 행렬
    D3D11Utils::CreateConstantBuffer(m_device, m_constantBufferData, m_constantBuffer);

    // Shader 생성
    
    // InputLayout의 데이터 형식으로 Vertex에 들어가는 데이터 구조체와 같게 설정
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        // COLOR가 시작되는 위치로 POSITION이 RGB 각각 4 byte씩 할당한 다음부터 시작하므로 4(byte) * 3(개수)
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 9, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    // InputLayout & Vertex Shader 생성
    D3D11Utils::CreateVSAndInputLayout(m_device, L"Shader/BasicVS.hlsl", inputElements,
        m_vertexShader, m_inputLayout);

    // Pixel Shader 생성
    D3D11Utils::CreatePS(m_device, L"Shader/BasicPS.hlsl", m_pixelShader);

	return true;
}

// CPU에서 물체가 어떻게 움직일지 행렬 계산 후 GPU로 전송
void Graphics::Update(float dt)
{
    // 모델의 변환 행렬
    m_constantBufferData.world = Matrix::CreateScale(0.5f) * Matrix::CreateTranslation(Vector3(0.f, 0.f, 1.0f));
    // DirectX는 Row-Major 사용하나 HLSL같은 Shader 프로그램은 Column-Major 사용
    m_constantBufferData.world = m_constantBufferData.world.Transpose(); // Row-Major -> Column-Major 변환

    // 시점 변환 - 원리 = 시점이 움직이는 반대로 세상의 모든 모델을 움직인다.
    m_constantBufferData.view =
        // (카메라 위치, 보는 방향, 카메라의 upVector)
        DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
    m_constantBufferData.view = m_constantBufferData.view.Transpose();

    // 투영 행렬
    const float aspect = AppBase::GetAspectRatio();
    if (m_usePerspectiveProjection)
    {
        // 시야각
        const float fovAngleY = 70.f * DirectX::XM_PI / 180.f;
        // 원근 투영
        m_constantBufferData.proj =
            // (시야각, 화면비율, Near-Plane, Far-Plane)
            DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.f);
    }
    else
    {
        // 정투영
        m_constantBufferData.proj =
            // (x 방향 범위 변수 2개, y 방향 범위 변수 2개, Near-Plane, Far-Plane)
            DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
    }
    m_constantBufferData.proj = m_constantBufferData.proj.Transpose();

    // Constant Buffer Data를 CPU -> GPU 복사
    D3D11Utils::UpdateBuffer(m_context, m_constantBufferData, m_constantBuffer);
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

    // Vertex Shader 설정
    m_context->VSSetShader(m_vertexShader.Get(), 0, 0);

    /*
        // 여러 constant buffer 사용시 
        ID3D11Buffer *pptr[1] = {
            m_constantBuffer.Get(),
        };
        m_context->VSSetConstantBuffers(0, 1, pptr);
    */ 
    // Vertex Shader의 Constant Buffer 설정
    // (0번 index부터 시작, 1개, constant buffer)
    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    // Pixel Shader 설정
    m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
    
    // Rasterizer State 설정
    if (m_wireFrame) // 
        m_context->RSSetState(m_WireRasterizerState.Get());
    else
        m_context->RSSetState(m_SolidRasterizerState.Get());

    // Vertex/Index Buffer 설정
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    
    // Input Layout 설정
    m_context->IASetInputLayout(m_inputLayout.Get());
    // Vertex/Index Buffer 설정
    m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Index Buffer가 가진 Vertex들의 연결관계 설정 (_TRIANGLESTRIP 등)
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 3개씩 묶어서 삼각형
    
    // GPU가 준비되면 Render 시작
    // (몇 개를 그릴지 지정, Buffer에서 몇 번쨰 index로부터 그리기 시작할 지 지정)
    m_context->DrawIndexed(m_indexCount, 0, 0);
}

void Graphics::UpdateGUI()
{
    ImGui::Checkbox("WireFrame", &m_wireFrame);
    ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
}
#include <iostream>
#include <vector>
#include <string>
// windows
#include <windows.h>
#include <wrl.h> // ComPtr

// ImGUI
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;
/* ����: ������ ���� ��� ���� �߻� ���

   // MS ����
   inline void ThrowIfFailed(HRESULT hr)
   {
	   if (FAILED(hr))
	   {
		   // Set a breakpoint on this line to catch Win32 API errors.
		   throw Platform::Exception::CreateException(hr);
	   }
   }

   // �Լ� ��� ��ũ�θ� ����� ����ϴ� ���
   // Luna DX12 ����
   #ifndef ThrowIfFailed
   #define ThrowIfFailed(x)                                              \
   {                                                                     \
	   HRESULT hr__ = (x);                                               \
	   std::wstring wfn = AnsiToWString(__FILE__);                       \
	   if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
   }
   #endif
   */
class AppBase {
public:
	AppBase();
	~AppBase();

	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Initialize();
	int Run();

	float GetAspectRatio() const;

	// Pure Virtual Function�̱⿡ �ڽ�Ŭ������ �Լ� ����
	virtual void UpdateGUI() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
protected:
	bool InitWindow();
	bool InitD3D();
	bool InitGUI();

	// Inputlayout & Vertex Shader ����
	void CreateVSAndInputLayout(
		const std::wstring& filename,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& vertexShader,
		ComPtr<ID3D11InputLayout>& inputLayout);

	// Pixel Shader ����
	void CreatePS(
		const std::wstring& filename,
		ComPtr<ID3D11PixelShader>& pixelShader);

	// Index Buffer ����
	void CreateIndexBuffer(
		const std::vector<uint16_t>& indices,
		ComPtr<ID3D11Buffer>& indexBuffer);
	
	// Vertex Buffer ����
	template<typename T_VERTEX>
	void CreateVertexBuffer(
		const std::vector<T_VERTEX>& vertices,
		ComPtr<ID3D11Buffer>& vertexBuffer);

	// Constant Buffer ����
	template <typename T_CONSTANT>
	void CreateConstantBuffer(
		const T_CONSTANT& constantBufferData,
		ComPtr<ID3D11Buffer>& constantBuffer);

	// Update Buffer Data 
	template <typename T_DATA>
	void UpdateBuffer(
		const T_DATA& bufferData,
		ComPtr<ID3D11Buffer>& buffer);

public:
	int m_screenWidth;
	int m_screenHeight;
	HWND m_mainWindow;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	// Depth/Stencil ����
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	D3D11_VIEWPORT m_screenViewPort;

};

template<typename T_VERTEX>
inline void AppBase::CreateVertexBuffer(const std::vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
{
	D3D11_BUFFER_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ���� x
	bDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size()); // �� �޸�
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = 0; // No CPU Acess
	bDesc.StructureByteStride = sizeof(T_VERTEX); // �� ��Ʈ ������ ������

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // �ʱ�ȭ
	// CPU�� �������� Pointer�� �̰����� ������ ���� ����
	vertexBufferData.pSysMem = vertices.data();
	// Vertex Buffer�� ���� buffer resource���� ������ Subresource�� ���� �� ����.
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	// GPU�� Buffer�� ����� �޸� �Ҵ� �޾ƿ���
	const HRESULT hr = m_device->CreateBuffer(
		&bDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateVertexBuffer() failed. " << std::hex << hr << "\n";
	}
}

template<typename T_CONSTANT>
inline void AppBase::CreateConstantBuffer(const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
{
	D3D11_BUFFER_DESC cdDesc;
	cdDesc.ByteWidth = sizeof(constantBufferData);
	// �� ������ ����� �����͸� �����Ƿ�
	cdDesc.Usage = D3D11_USAGE_DYNAMIC; 
	cdDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cdDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	cdDesc.MiscFlags = 0;
	cdDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cdData;
	cdData.pSysMem = &constantBufferData;	// constant data
	cdData.SysMemPitch = 0;
	cdData.SysMemSlicePitch = 0;

	auto hr = m_device->CreateBuffer(&cdDesc, &cdData, constantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateConstantBuffer() failed. " << std::hex << hr << "\n";
	}
}

template<typename T_DATA>
inline void AppBase::UpdateBuffer(const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer)
{
	if (!buffer)
	{
		std::cout << "UpdateBuffer() buffer was not initialized.\n";
	}

	// CPU ������ -> GPU ���۷� ����
	D3D11_MAPPED_SUBRESOURCE ms;
	// ������ ���� ���� (������ Lock �Լ��� �ٸ� ���μ��� ���� ����)
	m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	// GPU �޸𸮿� ������ ī��
	memcpy(ms.pData, &bufferData, sizeof(bufferData));
	// ���� ���� (������ UnLock)
	m_context->Unmap(buffer.Get(), NULL);
}

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <vector>
#include <cassert>
#include <complex>
#include <queue>
#include <utility>

#undef min
#undef max

bool g_running = true;
ID3D11Device* gp_device = nullptr;
ID3D11DeviceContext* gp_deviceContext = nullptr;
IDXGISwapChain* gp_swapChain = nullptr;
ID3D11InputLayout* gp_inputLayout = nullptr;
ID3D11VertexShader* gp_vertexShader = nullptr;
ID3D11GeometryShader* gp_geometryShader = nullptr;
ID3D11PixelShader* gp_pixelShader = nullptr;
ID3D11RasterizerState* gp_rasterizerState = nullptr;
ID3D11RenderTargetView* gp_RTV = nullptr;

const float PI = 3.1415926f;

struct Circle
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 scale;
	DirectX::XMFLOAT4 color;

	Circle(float x, float y, float r, DirectX::XMFLOAT4 c) : position(x, y, 0, 1), scale(r, r, 0, 1), color(c) {}
};

std::vector<Circle> circles;
std::vector<std::vector<int>> circleAdjacency;

static HRESULT CompileShader(LPCWSTR pathName, const char* model, ID3DBlob** blobOut)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		pathName,
		nullptr, nullptr,
		"main",
		model,
		shaderFlags,
		0,
		blobOut,
		&errorBlob
	);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			errorBlob->Release();
		}
		return hr;
	}
	if (errorBlob)
	{
		errorBlob->Release();
	}
	return hr;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);

        case WM_QUIT:
            g_running = false;
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool CircleCircleCheck(std::complex<float> const& z1, std::complex<float> const& z2, float k1, float k2)
{
	float r1 = 0.01f + 1 / k1;
	float r2 = 0.01f + 1 / k2;
	float sqrDist = (z2.real() - z1.real()) * (z2.real() - z1.real()) + (z2.imag() - z1.imag()) * (z2.imag() - z1.imag());
	float sqrRad = (r1 + r2) * (r1 + r2);
	return sqrDist <= sqrRad;
}

int power(int n, int p)
{
	int res = 1;
	for (; p; res *= n, p--);
	return res;
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
#pragma region CIRCLE_GENERATION

	// Don't put this very high.
	// 11 is max otherwise result of power function is too large and causes infinite circle creation.
	// Could fix this but the problem only asks for 10 so I'm not going to (right now).
	const int iterationCount = 10;

	// Initial circles
	float r2 = 2;
	float r1 = (3 * r2) / (3 + 2 * sqrtf(3));
	float d = (2 * r1 * sqrtf(3)) / 3.f;

	circles.emplace_back(Circle(0, 0, r2, DirectX::XMFLOAT4(1, 1, 1, 1)));
	circles.emplace_back(Circle(0, d, r1, DirectX::XMFLOAT4(0, 0, 1, 1)));
	circles.emplace_back(Circle(-sinf(2 * PI / 3) * d, cosf(2 * PI / 3) * d, r1, DirectX::XMFLOAT4(0, 0, 1, 1)));
	circles.emplace_back(Circle(-sinf(4 * PI / 3) * d, cosf(4 * PI / 3) * d, r1, DirectX::XMFLOAT4(0, 0, 1, 1)));

	std::queue<std::pair<int, std::pair<int, int>>> circleQueue;

	// First iteration tasks, make new circles with given circle indices
	circleQueue.emplace(std::make_pair(0, std::make_pair(1, 2)));
	circleQueue.emplace(std::make_pair(0, std::make_pair(2, 3)));
	circleQueue.emplace(std::make_pair(0, std::make_pair(3, 1)));
	circleQueue.emplace(std::make_pair(1, std::make_pair(2, 3)));

	// Each new circle will add 3 more circles to be created per circle created.
	for (int iter = 1; iter <= iterationCount; ++iter)
	{
		int taskCount = 4 * std::max(1, (power(3, (iter - 1))));
		for (int i = 0; i < taskCount; ++i)
		{
			std::pair<int, std::pair<int, int>> task = circleQueue.front();
			circleQueue.pop();

			bool outside = task.first == 0;
			float k1 = ((outside ? -1 : 1) / circles[task.first].scale.x);
			float k2 = 1 / circles[task.second.first].scale.x;
			float k3 = 1 / circles[task.second.second].scale.x;
			float k4 = std::max(k1 + k2 + k3 + 2 * sqrtf(k1 * k2 + k2 * k3 + k3 * k1), k1 + k2 + k3 - 2 * sqrtf(k1 * k2 + k2 * k3 + k3 * k1));

			std::complex<float> z1(circles[task.first].position.x, circles[task.first].position.y);
			std::complex<float> z2(circles[task.second.first].position.x, circles[task.second.first].position.y);
			std::complex<float> z3(circles[task.second.second].position.x, circles[task.second.second].position.y);

			// Calculate both possible circle locations
			std::complex<float> z4_1 = (z1 * k1 + z2 * k2 + z3 * k3 + 2.f * std::sqrt(k1 * k2 * z1 * z2 + k2 * k3 * z2 * z3 + k1 * k3 * z1 * z3)) / k4;
			std::complex<float> z4_2 = (z1 * k1 + z2 * k2 + z3 * k3 - 2.f * std::sqrt(k1 * k2 * z1 * z2 + k2 * k3 * z2 * z3 + k1 * k3 * z1 * z3)) / k4;

			float sqrLen1 = z4_1.real() * z4_1.real() + z4_1.imag() * z4_1.imag();
			float sqrLen2 = z4_2.real() * z4_2.real() + z4_2.imag() * z4_2.imag();

			std::complex<float> z4 = sqrLen1 > sqrLen2 ? z4_1 : z4_2;

			circles.emplace_back(Circle(z4.real(), z4.imag(), 1 / k4, DirectX::XMFLOAT4(iter * 0.35f * (iter % 3 == 0), iter * 0.35f * ((iter + 1) % 3 == 0), iter * 0.35f * ((iter + 2) % 3 == 0), 1)));

			int newCircleIndex = circles.size() - 1;

			// Create new circle creation tasks
			circleQueue.emplace(std::make_pair(task.first, std::make_pair(newCircleIndex, task.second.first)));
			circleQueue.emplace(std::make_pair(task.first, std::make_pair(newCircleIndex, task.second.second)));
			circleQueue.emplace(std::make_pair(newCircleIndex, std::make_pair(task.second.first, task.second.second)));
		}
	}

	float circleArea = PI * (circles[0].scale.x * circles[0].scale.x);
	float areaSum = 0;
	for (int i = 1; i < circles.size(); ++i)
	{
		areaSum += PI * (circles[i].scale.x * circles[i].scale.x);
	}

	// Inaccurate due to floating point error...
	float uncovered = circleArea - areaSum;
	float fraction = uncovered / circleArea;

#pragma endregion
#pragma region WINAPI_INIT

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int width = 900;
    int height = 900;

    HWND hWnd;

    // https://docs.microsoft.com/en-us/previous-versions/ms942860(v=msdn.10)
    WNDCLASS wc = {};

    // lpfnWndProc is ptr to windows callback
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;

    // Name of window class
    wc.lpszClassName = L"IterativeCirclePacking";

    // Register the window class
    RegisterClass(&wc);

    // Data for window creation
    LPCWSTR wndClassName = L"IterativeCirclePacking";  // Must match name given to WNDCLASS
    LPCWSTR wndTitle = L"Iterative Circle Packing";    // Window title

    DWORD wndStyle = WS_OVERLAPPED |		           //
                     WS_SYSMENU    |                   // Window Style https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
                     WS_VISIBLE    |                   //
                     WS_MAXIMIZEBOX;		           //

    DWORD exWndStyle = 0;                              // Extended Window Style https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles

    int winX = CW_USEDEFAULT;				           // Window X placement
    int winY = CW_USEDEFAULT;				           // Window Y placement

    // Create window 
    hWnd = CreateWindowExW(
        exWndStyle,							           // Extended Window Style https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
        wndClassName,						           // Window Class Name
        wndTitle,							           // Window Name 
        wndStyle,							           // Window Style https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
        winX, winY, width, height,			           // Window Placement and Size (x, y, x_size, y_size)
        NULL,								           // Not a child window, so no parent
        NULL,								           // No menu
        hInstance,							           // Instance handle
        NULL								           // Unused
    );

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

#pragma endregion

#pragma region DX11_INIT

	HRESULT hr = S_OK;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL featureLevelSelected;

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = clientRect.right;
	swapChainDesc.BufferDesc.Height = clientRect.bottom;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_CENTERED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	swapChainDesc.OutputWindow = hWnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = true;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&gp_swapChain,
		&gp_device,
		&featureLevelSelected,
		&gp_deviceContext
	);

	assert(!FAILED(hr));

	// Create RTV
	ID3D11Texture2D* backBuffer = nullptr;

	hr = gp_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	assert(!FAILED(hr));

	hr = gp_device->CreateRenderTargetView(backBuffer, 0, &gp_RTV);

	assert(!FAILED(hr));

	backBuffer->Release();

	// Create regular rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	hr = gp_device->CreateRasterizerState(&rasterizerDesc, &gp_rasterizerState);

	assert(!FAILED(hr));

	// Create input layout
	D3D11_INPUT_ELEMENT_DESC posDesc =
	{
		"POSITION",                                        
		0,												   
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,                                                 
		0,                                                 
		D3D11_INPUT_PER_VERTEX_DATA,                       
		0                                                  
	};

	D3D11_INPUT_ELEMENT_DESC scaleDesc =
	{
		"SCALE",                                      
		0,												
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,                                            
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,                  
		0                                             
	};

	D3D11_INPUT_ELEMENT_DESC colorDesc =
	{
		"COLOR",
		0,
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	};

	D3D11_INPUT_ELEMENT_DESC inputLayout[3] =
	{
		posDesc,
		scaleDesc,
		colorDesc
	};

	ID3DBlob* shaderBlob = nullptr;

	// Compile vertex shader
	hr = CompileShader(L"./VertexShader.hlsl", "vs_4_0", &shaderBlob);
	assert(!FAILED(hr));

	hr = gp_device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&gp_vertexShader
	);

	assert(!FAILED(hr));

	hr = gp_device->CreateInputLayout(
		inputLayout,
		_countof(inputLayout),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&gp_inputLayout
	);
	assert(!FAILED(hr));

	shaderBlob->Release();

	// Compile geometry shader
	hr = CompileShader(L"./GeometryShader.hlsl", "gs_4_0", &shaderBlob);
	assert(!FAILED(hr));

	hr = gp_device->CreateGeometryShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&gp_geometryShader
	);

	assert(!FAILED(hr));

	// Compile pixel shader
	hr = CompileShader(L"./PixelShader.hlsl", "ps_4_0", &shaderBlob);
	assert(!FAILED(hr));

	hr = gp_device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&gp_pixelShader
	);
	assert(!FAILED(hr));

	shaderBlob->Release();

	/* Initialize the Device Context */
	gp_deviceContext->RSSetState(gp_rasterizerState);
	gp_deviceContext->IASetInputLayout(gp_inputLayout);
	gp_deviceContext->VSSetShader(gp_vertexShader, nullptr, 0);
	gp_deviceContext->GSSetShader(gp_geometryShader, nullptr, 0);
	gp_deviceContext->PSSetShader(gp_pixelShader, nullptr, 0);

	D3D11_VIEWPORT vp = {};
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

	vp.Width = static_cast<FLOAT>(clientRect.right);
	vp.Height = static_cast<FLOAT>(clientRect.bottom);
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	gp_deviceContext->RSSetViewports(1, &vp);

#pragma endregion

    // Main loop
    while (g_running)
    {
		// Message handling
        MSG msg = {};

        if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		FLOAT clearColor[4] = {0.2f, 0.1f, 0.3f, 1.f};
		gp_deviceContext->ClearRenderTargetView(gp_RTV, clearColor);

		// Create vertex buffer for circles
		D3D11_BUFFER_DESC vbDesc = {};
		ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

		// Fill out vertex buffer description
		vbDesc.ByteWidth = static_cast<UINT>(sizeof(Circle) * circles.size());
		vbDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
		vbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;

		// Create buffer data
		D3D11_SUBRESOURCE_DATA vbInitData = {};
		ZeroMemory(&vbInitData, sizeof(D3D11_SUBRESOURCE_DATA));
		vbInitData.pSysMem = circles.data();

		ID3D11Buffer* vertexBuffer = nullptr;

		// Create vertex buffer (every frame...)
		hr = gp_device->CreateBuffer(&vbDesc, &vbInitData, &vertexBuffer);
		assert(!FAILED(hr));

		// Draw circles
		UINT stride = sizeof(Circle);
		UINT offset = 0;
		gp_deviceContext->OMSetRenderTargets(1, &gp_RTV, nullptr);
		gp_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		gp_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		gp_deviceContext->Draw(circles.size(), 0);

		vertexBuffer->Release();

		gp_swapChain->Present(0, 0);
    }
}
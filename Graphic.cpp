#include "Graphic.h"

float colors[] = {1.0f,1.0f, 1.0f, 0.0f};

bool Graphic::Show() {
	int max = this->vertexBuffers.vertexBuffer.size();
	
	UINT stride = sizeof(Vertex_pos);
	UINT offset = 0;

	for (int i = 0; i < max; i++) {
		if (!this->vertexBuffers.hide[i]){
			this->DeviceContext->IASetVertexBuffers(0, 1, this->vertexBuffers.vertexBuffer[i].GetAddressOf(), &stride, &offset);
			this->DeviceContext->IASetPrimitiveTopology(this->vertexBuffers.Primtive[i]);
			this->DeviceContext->Draw(3, 0);
		}

	}
	return true;
}

bool Graphic::RenderFrame() {
	this->DeviceContext->ClearRenderTargetView(this->TargetView.Get(), colors);


	this->DeviceContext->IASetInputLayout(this->Vertex.inputLayout.Get());
	
	this->DeviceContext->RSSetState(this->rasterizerState.Get());

	this->DeviceContext->ClearDepthStencilView(this->StencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->DeviceContext->OMGetDepthStencilState(this->StencilState.GetAddressOf(),  0);

	this->DeviceContext->VSSetShader(this->Vertex.shader.Get(), NULL, 0);
	this->DeviceContext->PSSetShader(this->Pixel.pixel_shader.Get(), NULL, 0);

	Show();


	this->swapchain->Present(1, 0);

	return true;
}

AdapterData::AdapterData(IDXGIAdapter* pAdapter) {
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->desc);
	if (FAILED(hr)) {
		Error::Log(hr, L"Adapter Error");
	}
}

std::vector<AdapterData> Get_Adapters() {

	IDXGIFactory* pFactory = nullptr;
	std::vector<AdapterData> Adapters;
	IDXGIAdapter* pAdapter;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr)) {
		Error::Log(hr, L"Get_Adapters Error");
		return Adapters;
	}

	int index = 0;
	
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter))) {
		Adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}
	return Adapters;
}



bool Graphic::CreateVertexBuffer(Vertex_pos* vertex, Microsoft::WRL::ComPtr<ID3D11Buffer>* vertexBuffers) {
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.ByteWidth = sizeof(Vertex_pos) * 0xf;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA DATA;
	ZeroMemory(&DATA, sizeof(D3D11_SUBRESOURCE_DATA));
	DATA.pSysMem = vertex;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &DATA, vertexBuffers->GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"CreateBuffer Error");
		return false;
	}

	return true;
}



bool Graphic::Iinialize(HWND hwnd, int width, int height) {
	std::vector<AdapterData> Adapters = Get_Adapters();
	
	if (Adapters.size()<1) {
		Error::Log(L"succees");
		return false;
	}

	//--------------------Crate SwapChain-----------------------------------------
	DXGI_SWAP_CHAIN_DESC swapChain;

	swapChain.BufferDesc.Width = width;
	swapChain.BufferDesc.Height= height;
	swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChain.BufferDesc.RefreshRate.Denominator = 1;
	swapChain.BufferDesc.RefreshRate.Numerator = 60;

	swapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapChain.SampleDesc.Count = 1;
	swapChain.SampleDesc.Quality = 0;

	swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChain.Windowed = TRUE;
	swapChain.OutputWindow = hwnd;
	swapChain.BufferCount = 1;
	swapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL levels[]= {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
	};


	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		Adapters[0].pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		NULL,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&swapChain,
		this->swapchain.GetAddressOf(),
		this->device.GetAddressOf(),
		NULL,
		this->DeviceContext.GetAddressOf()
	);


	if (FAILED(hr)) {
		Error::Log(hr, L"Create device or SwapChain error");
	}
	//--------------------Crate SwapChain-----------------------------------------




	//Create backBuffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"BackBuffer GetBuffer error");
	}
	// backBuffer Create



	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->TargetView.GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"CreateRenderTargetView error");
	}
	
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	this->DeviceContext->RSSetViewports(1, &viewport);
	this->DeviceContext->OMSetRenderTargets(1, this->TargetView.GetAddressOf(), nullptr);



	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"Color", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	LPCWSTR pixel_path = L"..\\x64\\Debug\\pixelshader.cso";
	LPCWSTR shader_path = L"..\\x64\\Debug\\shader.cso";

	UINT size = ARRAYSIZE(layout);

	this->Vertex.init(shader_path, this->device, layout, size);
	this->Pixel.init(pixel_path, this->device);



	

	D3D11_TEXTURE2D_DESC stencilDesc;

	stencilDesc.Width = width;
	stencilDesc.Height = height;
	stencilDesc.MipLevels = 1;
	stencilDesc.ArraySize = 1;
	stencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencilDesc.SampleDesc.Count = 1;
	stencilDesc.SampleDesc.Quality = 0;
	stencilDesc.Usage = D3D11_USAGE_DEFAULT;
	stencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	stencilDesc.CPUAccessFlags = 0;
	stencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&stencilDesc,
		NULL, this->ZBuffer.GetAddressOf());

	if (FAILED(hr)) {
		Error::Log(hr, L"CreateTexture2D Error");
		return false;
	}

	hr = this->device->CreateDepthStencilView(this->ZBuffer.Get(), NULL, this->StencilView.GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"CreateDepthStencilView Error");
		return false;
	}
	D3D11_DEPTH_STENCIL_DESC StencilDesc;
	ZeroMemory(&StencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	StencilDesc.DepthEnable = true;
	StencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	StencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;

	hr = this->device->CreateDepthStencilState(&StencilDesc, this->StencilState.GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"CreateDepthStencilState Error");
		return false;
	}

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;


	this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());

	return true;
} 




bool VertexShader::init(LPCWSTR path, Microsoft::WRL::ComPtr<ID3D11Device> device, D3D11_INPUT_ELEMENT_DESC* element, UINT size)
{
	HRESULT hr = D3DReadFileToBlob(path, this->shader_buffer.GetAddressOf());
	if (FAILED(hr)){
		Error::Log(hr, L"D3DReadFileToBlob Error");
		return false;
	}

	hr = device->CreateVertexShader(this->shader_buffer->GetBufferPointer(), 
		this->shader_buffer->GetBufferSize(), 
		NULL, this->shader.GetAddressOf());
	
	if (FAILED(hr)) {
		Error::Log(hr, L"CreateVertexShader Error");
		return false;
	}


	hr=device->CreateInputLayout(element, size, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"Create InputLayout Error");
		return false;
	}


	return true;
}

bool Pixel::init(LPCWSTR path, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	HRESULT hr = D3DReadFileToBlob(path, this->shader_buffer.GetAddressOf());
	if (FAILED(hr)) {
		Error::Log(hr, L"Pixel D3DReadFileToBlob Error");
		return false;
	}

	hr = device->CreatePixelShader(this->shader_buffer->GetBufferPointer(),
		this->shader_buffer->GetBufferSize(),
		nullptr, this->pixel_shader.GetAddressOf());

	if (FAILED(hr)) {
		Error::Log(hr, L"Create PixelShader Error");
		return false;
	}


	return true;
}

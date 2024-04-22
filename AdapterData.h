#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>



class AdapterData {
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desc;
};


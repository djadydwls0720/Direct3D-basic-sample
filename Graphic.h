#pragma once
#include <Windows.h>
#include <vector>
#include "AdapterData.h"
#include "ErrorLog.h"
#include "wrl/client.h"
#include "DirectXMath.h"
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

struct Vertex_pos {
	Vertex_pos();
	Vertex_pos(float x, float y, float z, float r, float g, float b) :
		pos(x, y, z),color(r, g, b) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

struct D3Figure {
	D3Figure();
	D3Figure(Vertex_pos* Poitions, D3D_PRIMITIVE_TOPOLOGY TOPOLOGY) : Poitions(Poitions), TOPOLOGY(TOPOLOGY) {}
	Vertex_pos* Poitions;
	D3D_PRIMITIVE_TOPOLOGY TOPOLOGY;
};

struct VertexBuffers {
	//VertexBuffers();
	//VertexBuffers(
	//	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> vertexBuffer, 
	//	std::vector<D3D_PRIMITIVE_TOPOLOGY> Primtive,
	//	std::vector<BOOL> hide);
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> vertexBuffer;
	std::vector<D3D_PRIMITIVE_TOPOLOGY> Primtive;
	std::vector<BOOL> hide;
};


class VertexShader {
public:
	bool init(LPCWSTR path, Microsoft::WRL::ComPtr<ID3D11Device> device, D3D11_INPUT_ELEMENT_DESC* element, UINT size);
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;

private:

};

class Pixel {
public:
	bool init(LPCWSTR path, Microsoft::WRL::ComPtr<ID3D11Device> device);
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer = nullptr;
private:

};

class Graphic {
public:
	bool Iinialize(HWND hwnd, int width, int height);
	bool RenderFrame();
	bool Show();
	bool Line(float x, float y, float z);
	bool CreateVertexBuffer(Vertex_pos* vertex, Microsoft::WRL::ComPtr<ID3D11Buffer>* vertexBuffers);

	VertexShader Vertex;
	Pixel Pixel;
	VertexBuffers vertexBuffers;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> TargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> ZBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> StencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> StencilState;
	//Vertex_pos *vertex;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

};
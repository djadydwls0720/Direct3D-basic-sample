#include "Engine.h"

bool Engine::RenderFrame() {
	this->Graphic.RenderFrame();
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

bool Window_Application(HINSTANCE hInstance, LPCWSTR TitleName, LPCWSTR ClassName, int Width, int Height) {
	WNDCLASS WndClass;
	HINSTANCE c = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = NULL;
	WndClass.hInstance = hInstance;
	WndClass.lpszClassName = ClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.lpfnWndProc = WndProc;
	RegisterClass(&WndClass);


	return true;
}



bool Engine::Initialize(HINSTANCE hInstance, LPCWSTR TitleName, LPCWSTR ClassName, int Width, int Height) {
	if (Window_Application(hInstance, TitleName, ClassName, Width, Height)) {

		this->handle = CreateWindowEx(0, ClassName, TitleName,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 100, 0, Width,Height,
			NULL, (HMENU)NULL, hInstance, NULL);
		ShowWindow(this->handle, SW_SHOW);
	}

	if (Graphic.Iinialize(this->handle, Width, Height)) {

	}

	return true;
}

bool Engine::ProcMessage() {

	MSG Message;
	ZeroMemory(&Message, sizeof(MSG));

	while (PeekMessage(&Message, this->handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	
	if (WM_QUIT == Message.message) {
		if (!IsWindow(this->handle)) {
			this->handle = NULL;
			UnregisterClass(this->ClassName, this->hInstance);
			return false;
		}

	}
	return true;
}
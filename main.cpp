#include "Engine.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {

	Engine window;
	Line_Pos line_pos = Line_Pos(Postion(0.05f, 0.0f, 0.1f), Color(1, 1, 1), Postion(0.0f, 0.5f, 0.1f), Color(1, 1, 1));
	Line_Pos line2_pos = Line_Pos(Postion(0.0f, 0.0f, 0.1f), Color(1, 1, 1), Postion(0.5f, 0.5f, 0.1f), Color(1, 1, 1));
	
	bool running = window.Initialize(hInstance, L"First", L"First", 1000, 1000);
	Line line = Line(&window, line_pos ,0);
	Line line2 = Line(&window, line2_pos,1);
	line2.Hide();
	int a = 0;

	if (running) {
		while (window.ProcMessage()) {
			line.Draw(line_pos);
			window.RenderFrame();
		}
	}

}
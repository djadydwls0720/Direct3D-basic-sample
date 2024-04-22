#pragma once
#include "Graphic.h"



class Engine {
public:
	bool Initialize(HINSTANCE hInstance, const LPCWSTR TitleName, const LPCWSTR ClassName, int Width, int Height);
	bool Update();
	bool ProcMessage();
	bool RenderFrame();
	Graphic Graphic;

	
private:
	HINSTANCE hInstance = hInstance;
	LPCWSTR TitleName=NULL;
	LPCWSTR ClassName = NULL;
	HWND handle = NULL;
};


class Color {
public:
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	float r;
	float g;
	float b;
};

class Postion {
public:
	Postion(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
};

class Line_Pos {
public:
	Line_Pos(Postion pos1,
		Color color1,
		Postion pos2,
		Color color2) : pos1(pos1), pos2(pos2), color1(color1), color2(color2) {}

	Postion pos1;
	Color color1;
	Postion pos2;
	Color color2;
};

class Line {
public:

	Engine* window;
	Line_Pos LinePos;
	bool hide = false;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	int index;


	Line(Engine* w, Line_Pos LinePos, int index) :
		window(w), LinePos(LinePos), index(index) {

		Vertex_pos Vertexs[] = { Vertex_pos(LinePos.pos1.x, LinePos.pos1.y, LinePos.pos1.z, LinePos.color1.r, LinePos.color1.g, LinePos.color1.b),
							 Vertex_pos(LinePos.pos2.x, LinePos.pos2.y, LinePos.pos2.z, LinePos.color2.r, LinePos.color2.g, LinePos.color2.b) };
		w->Graphic.CreateVertexBuffer(Vertexs, &vertexBuffer);
		w->Graphic.vertexBuffers.vertexBuffer.push_back(vertexBuffer);
		w->Graphic.vertexBuffers.Primtive.push_back(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		w->Graphic.vertexBuffers.hide.push_back(false);
	}

	bool Draw(Line_Pos LinePos);
	void Hide();
	void Seek();
};
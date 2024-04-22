#include "Engine.h"

bool Line::Draw(Line_Pos LinePos) {
	D3D11_MAPPED_SUBRESOURCE mappend;
	ZeroMemory(&mappend, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
	Vertex_pos Vertexs[] = { Vertex_pos(LinePos.pos1.x, LinePos.pos1.y, LinePos.pos1.z, 
		LinePos.color1.r, LinePos.color1.g, LinePos.color1.b),
					Vertex_pos(LinePos.pos2.x, LinePos.pos2.y, LinePos.pos2.z,
		LinePos.color2.r, LinePos.color2.g, LinePos.color2.b) };

	//auto m_d3dContext = 
	this->window->Graphic.DeviceContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD,
		0, &mappend);
	memcpy(mappend.pData, Vertexs, sizeof(Vertexs));

	this->window->Graphic.DeviceContext->Unmap(this->vertexBuffer.Get(), 0);

	return true;
}

void Line::Hide() {// God Hide on bush
	this->window->Graphic.vertexBuffers.hide[this->index] = true;
}
void Line::Seek() {// God Hide on bush
	this->window->Graphic.vertexBuffers.hide[this->index] = false;
}

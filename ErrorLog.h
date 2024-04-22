#pragma once
#include "WIndows.h"
#include "iostream"

class Error{
public:
	static void Log(HRESULT hr, std::wstring message);
	static void Log(std::wstring message);
};
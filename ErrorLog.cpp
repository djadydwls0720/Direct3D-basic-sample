#include "ErrorLog.h"
#include<comdef.h>

void Error::Log(std::wstring message) {
	MessageBoxW(NULL, message.c_str(), L"Error", NULL);
}

void Error::Log(HRESULT hr, std::wstring message) {
	_com_error error(hr);
	std::wstring Msg = message + L'\n' + error.ErrorMessage();
	MessageBoxW(NULL, Msg.c_str(), L"Error", NULL);
}
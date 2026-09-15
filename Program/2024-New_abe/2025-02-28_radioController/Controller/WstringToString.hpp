#pragma once

#include<cassert>
#include<string>
#include<Windows.h>

std::wstring StringToWstring(const std::string& str);
std::string WstringToString(const std::wstring& wstr);
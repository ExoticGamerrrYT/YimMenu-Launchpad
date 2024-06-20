#pragma once

#include <Windows.h>

DWORD GetProcId(const char* procName);
bool InjectDLL(const char* dllPath, const char* procName);

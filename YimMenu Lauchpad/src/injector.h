#pragma once
#ifndef DLL_INJECTOR_H
#define DLL_INJECTOR_H

#include <Windows.h>

DWORD GetProcId(const char* procName);
bool InjectDLL(const char* dllPath, const char* procName);

#endif // DLL_INJECTOR_H

#include "injector.h"
#include <iostream>
#include <TlHelp32.h>

DWORD GetProcId(const char* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to create toolhelp snapshot. Error code: " << GetLastError() << std::endl;
        return 0;
    }

    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);

    if (!Process32First(hSnap, &procEntry))
    {
        std::cerr << "Failed to retrieve the first process in the snapshot. Error code: " << GetLastError() << std::endl;
        CloseHandle(hSnap);
        return 0;
    }

    do
    {
        if (!_stricmp(procEntry.szExeFile, procName))
        {
            procId = procEntry.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnap, &procEntry));

    CloseHandle(hSnap);
    return procId;
}

bool InjectDLL(const char* dllPath, const char* procName)
{
    try
    {
        DWORD procId = 0;
        const int maxRetries = 5; // Maximum number of retries
        int attempts = 0;

        while (!procId && attempts < maxRetries)
        {
            procId = GetProcId(procName);
            if (!procId)
            {
                std::cerr << "Process not found: " << procName << ". Retrying in 30ms..." << std::endl;
                Sleep(30);
                attempts++;
            }
        }

        if (!procId)
        {
            std::cerr << "Process not found after " << maxRetries << " attempts." << std::endl;
            return false;
        }

        HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
        if (!hProc || hProc == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Failed to open process: " << procName << " with ID " << procId << ". Error code: " << GetLastError() << std::endl;
            return false;
        }

        void* loc = VirtualAllocEx(hProc, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!loc)
        {
            std::cerr << "Failed to allocate memory in target process. Error code: " << GetLastError() << std::endl;
            CloseHandle(hProc);
            return false;
        }

        if (!WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, NULL))
        {
            std::cerr << "Failed to write DLL path to target process memory. Error code: " << GetLastError() << std::endl;
            VirtualFreeEx(hProc, loc, 0, MEM_RELEASE);
            CloseHandle(hProc);
            return false;
        }

        HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, NULL);
        if (!hThread)
        {
            std::cerr << "Failed to create remote thread in target process. Error code: " << GetLastError() << std::endl;
            VirtualFreeEx(hProc, loc, 0, MEM_RELEASE);
            CloseHandle(hProc);
            return false;
        }

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        VirtualFreeEx(hProc, loc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "An unknown exception occurred." << std::endl;
        return false;
    }
}

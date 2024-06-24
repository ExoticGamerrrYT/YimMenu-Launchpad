#include <iostream>
#include <string>
#include <windows.h>

void LaunchGTA(const std::string& url) {
    // Convert std::string to std::wstring
    std::wstring wide_url(url.begin(), url.end());

    // Use ShellExecute
    HINSTANCE result = ShellExecuteW(NULL, L"open", wide_url.c_str(), NULL, NULL, SW_SHOWNORMAL);

    // Verify
    if ((int)result > 32) {
        std::wcout << L"GTA V launched correctly." << std::endl;
    }
    else {
        std::wcerr << L"Error when launching GTA V: " << (int)result << std::endl;
    }
}

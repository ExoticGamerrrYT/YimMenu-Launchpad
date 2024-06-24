#include <iostream>
#include <string>
#include <windows.h>

// Función para lanzar GTA V usando ShellExecute
void LaunchGTA(const std::string& url) {
    // Convertir std::string a std::wstring
    std::wstring wide_url(url.begin(), url.end());

    // Usar ShellExecute para abrir la URL
    HINSTANCE result = ShellExecuteW(NULL, L"open", wide_url.c_str(), NULL, NULL, SW_SHOWNORMAL);

    // Verificar el resultado
    if ((int)result > 32) {
        std::wcout << L"GTA V lanzado correctamente." << std::endl;
    }
    else {
        std::wcerr << L"Error al intentar lanzar GTA V: " << (int)result << std::endl;
    }
}

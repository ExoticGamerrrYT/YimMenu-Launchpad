#ifndef UPDATER_H
#define UPDATER_H

#include <string>

namespace Updater {
    extern std::string appDataPath;     // Variable global para la ruta de %APPDATA%
    extern std::string folder;          // Carpeta en %APPDATA%
    extern std::string hashFilePath;    // Ruta del archivo hash.txt
    extern std::string savedHash;       // Hash guardado
    extern std::string releaseUrl;      // URL de la página de lanzamiento
    extern std::string releasePage;     // Contenido de la página de lanzamiento
    extern std::string expectedHash;    // Hash esperado
    extern std::string dllUrl;          // URL del archivo DLL
    extern std::string dllPath;         // Ruta del archivo DLL

    void UpdateYimMenu();

} // namespace Updater

#endif // UPDATER_H

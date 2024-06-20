#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

const char* appdata = std::getenv("APPDATA");
fs::path yimMenuDir = fs::path(appdata) / "YimMenu";
fs::path hashFile = yimMenuDir / "hash.txt";
fs::path dllFile = yimMenuDir / "YimMenu.dll";

// Función para escribir los datos recibidos por CURL en un std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Función para escribir los datos recibidos por CURL en un archivo
static size_t WriteFileCallback(void* ptr, size_t size, size_t nmemb, void* stream) {
    std::ofstream* outFile = static_cast<std::ofstream*>(stream);
    size_t totalSize = size * nmemb;
    outFile->write(static_cast<const char*>(ptr), totalSize);
    return totalSize;
}

// Función para obtener el contenido de una URL
std::string getURLContent(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

// Función para extraer el SHA-256 de la página
std::string extractSHA256(const std::string& pageContent) {
    std::regex sha256_regex(R"(([a-f0-9]{64})\s+YimMenu\.dll)");
    std::smatch match;
    if (std::regex_search(pageContent, match, sha256_regex)) {
        return match[1].str();
    }
    else {
        return "SHA-256 hash not found";
    }
}

// Función para leer el hash guardado en el archivo hash.txt
std::string readSavedHash(const fs::path& hashFile) {
    std::ifstream inFile(hashFile);
    if (inFile.is_open()) {
        std::string savedHash;
        std::getline(inFile, savedHash);
        inFile.close();
        return savedHash;
    }
    return "";
}

// Función para guardar el hash en el archivo hash.txt
void saveHash(const fs::path& hashFile, const std::string& hash) {
    std::ofstream outFile(hashFile);
    if (outFile.is_open()) {
        outFile << hash;
        outFile.close();
    }
    else {
        std::cerr << "Error: Unable to write hash file." << std::endl;
    }
}

// Nueva función para descargar un archivo desde una URL y guardarlo en la ruta especificada
bool downloadFile(const std::string& url, const std::string& outFilename) {
    CURL* curl;
    CURLcode res;
    std::ofstream outFile(outFilename, std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << outFilename << std::endl;
        return false;
    }

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects if necessary

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_cleanup(curl);
    }

    outFile.close();
    return true;
}
/*
void CheckDirs()
{
    std::string savedHash;

    if (!fs::exists(yimMenuDir)) {
        fs::create_directories(yimMenuDir);
    }
    if (fs::exists(hashFile)) {
        savedHash = readSavedHash(hashFile);
    }
}
*/

void checkAndCreateFile(const std::string& hash) {
    const char* appdata = std::getenv("APPDATA");
    if (!appdata) {
        std::cerr << "Error: APPDATA environment variable not found." << std::endl;
        return;
    }

    fs::path yimMenuDir = fs::path(appdata) / "YimMenu";
    fs::path hashFile = yimMenuDir / "hash.txt";
    fs::path dllFile = yimMenuDir / "YimMenu.dll";
    std::string savedHash;

    // Crear carpeta si no existe
    if (!fs::exists(yimMenuDir)) {
        fs::create_directories(yimMenuDir);
    }

    // Leer el hash guardado
    if (fs::exists(hashFile)) {
        savedHash = readSavedHash(hashFile);
    }

    // Comprobar si el hash ha cambiado o si el DLL no existe
    if (savedHash != hash || !fs::exists(dllFile)) {
        std::cout << "Downloading YimMenu.dll..." << std::endl;
        if (downloadFile("https://github.com/YimMenu/YimMenu/releases/download/nightly/YimMenu.dll", dllFile.string())) {
            saveHash(hashFile, hash);
        }
        else {
            std::cerr << "Error: Failed to download YimMenu.dll" << std::endl;
        }
    }
    else {
        std::cout << "YimMenu.dll is up to date." << std::endl;
    }
}

// Función para comprobar y crear la carpeta y archivo



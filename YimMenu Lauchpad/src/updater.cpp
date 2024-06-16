#include "updater.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
#include <regex>

namespace fs = std::filesystem;

// Variables globales definidas externamente
namespace Updater {
    std::string appDataPath;
    std::string folder;
    std::string hashFilePath;
    std::string savedHash;
    std::string releaseUrl = "https://github.com/YimMenu/YimMenu/releases/tag/nightly";
    std::string releasePage;
    std::string expectedHash;
    std::string dllUrl = "https://github.com/YimMenu/YimMenu/releases/download/nightly/YimMenu.dll";
    std::string dllPath;
}

// Function to write data from curl into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    }
    catch (std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

// Function to download data from a URL
std::string downloadData(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response_string;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Set User-Agent to mimic a browser request
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

        // Follow redirections if any
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Enable SSL verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return response_string;
}

// Function to download a file from a URL
void downloadFile(const std::string& url, const std::string& filePath) {
    CURL* curl;
    CURLcode res;
    FILE* fp;
    errno_t err;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        err = fopen_s(&fp, filePath.c_str(), "wb");
        if (err != 0) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Set User-Agent to mimic a browser request
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

        // Follow redirections if any
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Enable SSL verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        fclose(fp);
    }
    curl_global_cleanup();
}

// Function to extract SHA256 hash from the release page
std::string extractSHA256FromReleasePage(const std::string& pageContent) {
    std::regex sha256_regex(R"([a-f0-9]{64})");
    std::smatch match;
    if (std::regex_search(pageContent, match, sha256_regex)) {
        return match.str();
    }
    else {
        throw std::runtime_error("No SHA256 hash found on the release page");
    }
}

// Function to update YimMenu
void Updater::UpdateYimMenu() {
    // Obtain the path of %APPDATA%
    char* rawAppDataPath = nullptr;
    size_t requiredSize;
    errno_t err = _dupenv_s(&rawAppDataPath, &requiredSize, "APPDATA");
    if (err != 0 || rawAppDataPath == nullptr) {
        std::cerr << "Error obtaining the path of %APPDATA%" << std::endl;
        return;
    }
    appDataPath = rawAppDataPath;
    std::cout << appDataPath << std::endl;
    free(rawAppDataPath);

    // Create folder in %APPDATA%
    folder = appDataPath + "/YimMenu";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    std::cout << folder << std::endl;

    // Create hash.txt if it doesn't exist
    hashFilePath = folder + "/hash.txt";
    if (!fs::exists(hashFilePath)) {
        std::ofstream hashFile(hashFilePath);
        hashFile.close();
    }

    std::cout << hashFilePath << std::endl;

    // Read current hash
    std::ifstream hashFile(hashFilePath);
    std::getline(hashFile, savedHash);
    hashFile.close();

    // Download and extract SHA256 from release page
    releasePage = downloadData(releaseUrl);
    expectedHash = extractSHA256FromReleasePage(releasePage);
    std::cout << "Saved hash: " << savedHash << std::endl;
    std::cout << "Expected hash: " << expectedHash << std::endl;

    // Compare hashes
    dllPath = folder + "/YimMenu.dll"; // Set the dllPath before comparison
    if (savedHash != expectedHash) {
        // Download YimMenu.dll if hash is different
        downloadFile(dllUrl, dllPath);

        // Save new hash in hash.txt
        std::ofstream hashFileOut(hashFilePath);
        hashFileOut << expectedHash;
        hashFileOut.close();
    }

    // Verify if YimMenu.dll doesn't exist and download if necessary
    if (!fs::exists(dllPath)) {
        downloadFile(dllUrl, dllPath);
    }

    std::cout << "DLL Path: " << dllPath << std::endl; // Print the dllPath
    std::cout << "Process completed." << std::endl;
}

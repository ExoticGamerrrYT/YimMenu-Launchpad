#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::string getURLContent(const std::string& url);
std::string extractSHA256(const std::string& pageContent);
void checkAndCreateFile(const std::string& hash);

extern const char* appdata;
extern fs::path yimMenuDir;
extern fs::path hashFile;
extern fs::path dllFile;

#pragma once

#include <iostream>

std::string getURLContent(const std::string& url);
std::string extractSHA256(const std::string& pageContent);
void checkAndCreateFile(const std::string& hash);
#pragma once

#include <string>

namespace utils {
std::string exec(const char *cmd);
void save(const std::string &filePath);
void saveSong(const std::string &filePath);
} // namespace utils

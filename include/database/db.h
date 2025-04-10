#pragma once

#include <memory>

// TODO: update with postgres option in the future
enum class TypeDB { sqlite, postgres, unknown };

namespace SQLite {
class Database;
};

TypeDB hashString(const std::string &typeDB);
std::unique_ptr<SQLite::Database> NewDBClient();

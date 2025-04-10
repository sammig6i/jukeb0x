#pragma once

#include <memory>

namespace SQLite {
class Database;
};

namespace db {
// TODO: update with postgres option in the future
enum class TypeDB { sqlite, postgres, unknown };

TypeDB hashString(const std::string &typeDB);
std::unique_ptr<SQLite::Database> NewDBClient();

}; // namespace db

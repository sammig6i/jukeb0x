#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <jukeb0x/database/db.h>
#include <jukeb0x/database/sqlite.h>
#include <yaml-cpp/yaml.h>

namespace db {

TypeDB hashString(const std::string &typeDB) {
  if (typeDB == "sqlite")
    return TypeDB::sqlite;

  if (typeDB == "postgres")
    return TypeDB::postgres;

  return TypeDB::unknown;
}

std::unique_ptr<SQLite::Database> NewDBClient() {
  YAML::Node config;
  try {
    config = YAML::LoadFile("config.yaml");
  } catch (YAML::Exception &e) {
    std::cerr << "Error loading config file: " << e.what() << std::endl;
    throw;
  }

  std::string typeDB_str;

  if (config["database"] && config["database"]["type"] &&
      !config["database"]["type"].IsNull()) {
    typeDB_str = config["database"]["type"].as<std::string>();

  } else {
    typeDB_str = "sqlite";
  }

  TypeDB typeDB = hashString(typeDB_str);
  std::string databasePath;

  switch (typeDB) {
  case TypeDB::sqlite:
    databasePath = config["database"]["path"].as<std::string>();
    return NewSQLiteClient(databasePath);
    break;

  default:
    std::cerr << "Unsupported Database type: " + typeDB_str << std::endl;
    return nullptr;
  }
}

} // namespace db

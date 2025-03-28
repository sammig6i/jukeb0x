#include "db.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <iostream>
#include <yaml-cpp/yaml.h>

std::unique_ptr<SQLite::Database> NewSQLiteClient(int flags) {
  try {
    // TODO update to follow the GetDBPath function and work with the
    // config.yaml file
    std::string dbPath = GetDBPath();

    std::filesystem::path path = dbPath;

    if (path.has_parent_path()) {
      std::filesystem::create_directories(path.parent_path());
    }

    auto db_ptr = std::make_unique<SQLite::Database>(path.string(), flags);

    CreateTables(db_ptr);

    return db_ptr;
  } catch (SQLite::Exception &e) {
    std::cerr << "SQLite error: " << e.what() << std::endl;
    throw;
  }
}

void CreateTables(std::unique_ptr<SQLite::Database> &db) {
  if (!db) {
    throw std::runtime_error("Database pointer is null");
  }

  SQLite::Statement songsTable(*db, "CREATE TABLE IF NOT EXISTS songs ("

                                    "title TEXT NOT NULL, "
                                    "artist TEXT NOT NULL, "
                                    "ytID TEXT, "
                                    "key TEXT NOT NULL UNIQUE, "
                                    "spectrogram BLOB"
                                    ")");

  try {
    songsTable.exec();
  } catch (SQLite::Exception &e) {
    std::cerr << "SQLite error creating table 'songs': " << e.what()
              << std::endl;
    throw;
  } catch (std::exception &e) {
    std::cerr << "Standard error during table creation: " << e.what()
              << std::endl;
    throw;
  }
}

std::string GetDBPath(const YAML::Node &config) {
  try {
    if (config["database"] && config["database"]["path"] &&
        !config["database"]["path"].IsNull()) {
      std::string configPath = config["databe"]["path"].as<std::string>();
      std::cout << "Using db path from config file: " << configPath
                << std::endl;
      return configPath;
    }
  } catch (const YAML::Exception &e) {
    std::cerr << "Error acessing db path in config.yaml: " << e.what()
              << std::endl;
  }
  std::string defaultPath = "data/music.db";
  std::cout << "Using default db path: " << defaultPath << std::endl;

  return defaultPath;
}

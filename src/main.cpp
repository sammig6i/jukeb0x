#include "db.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <stdlib.h>
#include <yaml-cpp/yaml.h>

int main(int argc, char **argv) {
  std::unique_ptr<SQLite::Database> db;
  YAML::Node config = YAML::LoadFile("config.yaml");

  try {
    db = NewSQLiteClient(config);
    std::cout << "DB init sucessful" << std::endl;

    std::cout << "Database file located at: " << db->getFilename() << std::endl;

    if (db) {
      std::cout << "Databse connection pointer is valid." << std::endl;
    } else {
      std::cerr << "Error: Database pointer is null after sucessful return"
                << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Ready for database operations" << std::endl;

  } catch (std::exception &e) {
    std::cerr << "FATAL: Database init failed: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

#include "database/db.h"
#include "utils/utils.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <yaml-cpp/yaml.h>
namespace fs = std::filesystem;

std::string CONFIG_PATH = "config.yaml";

int main(int argc, char **argv) {
  std::unique_ptr<SQLite::Database> db;
  try {
    db = NewDBClient();
    if (db != nullptr) {
      std::cout << "DB COnnection success: " << db->getFilename() << std::endl;
    }

  } catch (SQLite::Exception &e) {
    std::cout << "Error with DB: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  fs::path file("test_data/audio_source/big_tymers_still_fly.mp3");
  fs::path dir("test_data/audio_source/");

  std::cout << "File" << std::endl;
  save(file);

  std::cout << std::endl;

  std::cout << "Directory" << std::endl;
  save(dir);

  return EXIT_SUCCESS;
}

#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <jukeb0x/database/db.h>
#include <jukeb0x/utils/utils.h>
#include <yaml-cpp/yaml.h>
namespace fs = std::filesystem;

// TODO: make tests for code written so far
std::string CONFIG_PATH = "config.yaml";

int main() {
  std::unique_ptr<SQLite::Database> db;
  try {
    db = db::NewDBClient();
    if (db != nullptr) {
      std::cout << "DB COnnection success: " << db->getFilename() << std::endl;
    }

  } catch (SQLite::Exception &e) {
    std::cout << "Error with DB: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  fs::path file("test_data/audio_source/big_tymers_still_fly.mp3");
  fs::path dir("test_data/audio_source/");

  std::cout << "-----File-----" << std::endl;
  utils::save(file);

  std::cout << std::endl;

  std::cout << "-----Directory-----" << std::endl;
  utils::save(dir);

  return EXIT_SUCCESS;
}

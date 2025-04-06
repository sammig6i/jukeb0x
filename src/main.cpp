#include "database/db.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <unordered_set>
#include <yaml-cpp/yaml.h>
namespace fs = std::filesystem;

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

  // TODO: use million songs subset data for songs
  YAML::Node config = YAML::LoadFile("config.yaml");
  std::unordered_set<std::string> extSet{".mp3", ".flac", ".m4a",
                                         ".ogg", ".wav",  ".webm"};

  for (const auto &dir : config["scan_directories"]) {
    // get extension of the audio file (.mp3, .flac, .m4a, etc)
    // check if the audio extension is in the set
    // if extension is a .wav file, make it consistent with all other wav files
    // but do not convert convert each non .wav extension with ffmpeg
    // add entry into database

    const fs::path path(dir.as<std::string>());

    int numWavFiles = 0;
    int notWavFiles = 0;
    for (const auto &entry : fs::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        std::string ext = fs::path(entry).extension();

        if (!extSet.count(ext)) {
          std::cout << "Not a valid extension: " << ext << std::endl;
        }

        if (ext.compare(".wav") == 0) {
          std::cout << "This is a wav file: " << ext << std::endl;
          numWavFiles++;
        } else {
          std::cout << "Not a wav file: " << ext << std::endl;
          notWavFiles++;
        }
      }
    }
    std::cout << "# of wav files: " << numWavFiles << std::endl;
    std::cout << "# NOT wav files: " << notWavFiles << std::endl;
  }

  return EXIT_SUCCESS;
}

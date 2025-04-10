#include "utils/utils.h"
#include <filesystem>
#include <iostream>
#include <string>
namespace fs = std::filesystem;

void save(const std::string &filePath) {
  std::error_code ec;

  fs::path file = fs::absolute(filePath, ec);
  if (ec) {
    std::cerr << "Error getting absolute path of file: " << ec.message()
              << std::endl;
    return;
  }

  if (fs::is_directory(file, ec)) {
    if (ec) {
      std::cerr << "Error checking if path is a directory: " << ec.message()
                << std::endl;
      return;
    }

    std::cout << "file path is a directory: " << file << std::endl;
    std::cout << "Savings songs from the directory: " << file << std::endl;

    for (const auto &entry : fs::directory_iterator(file)) {
      if (entry.is_regular_file(ec)) {
        std::cout << "File in directory " << file << ": " << entry.path()
                  << std::endl;
        saveSong(entry.path());
      }

      if (ec) {
        std::cout << "Error saving song " << entry.path() << ": "
                  << ec.message() << std::endl;
      }
    }
  } else if (fs::is_regular_file(file, ec)) {
    if (ec) {
      std::cerr << "Error checking if path is a regular file: " << ec.message()
                << std::endl;
      return;
    }

    std::cout << "file is a regular file: " << file << std::endl;
    saveSong(file);
  } else {
    std::cout << "File path is neither a regular file nor a directory: " << file
              << std::endl;
  }
}

void saveSong(const std::string &filePath) {
  /*
  * TODO: handle converting, saving and processing the song in .wav format
  * 1. get metadata from file using ffprobe
  * 2. create model for the track to save the important metadata
  * use the tags from the metadata to get track title and artist information
  * get the Youtube ID of the audio file
  * process the file by creating the mel spectrogram operations then store the
  spectrogram to the database
  * move the new wav file to the processed songs directory

  */
  std::cout << "Saving song: " << filePath << std::endl;
}

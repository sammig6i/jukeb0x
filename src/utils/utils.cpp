#include <array>
#include <filesystem>
#include <iostream>
#include <jukeb0x/utils/utils.h>
#include <jukeb0x/wav/wav.h>
#include <string>
namespace fs = std::filesystem;

namespace utils {

// execute shell commands
std::string exec(const char *cmd) {
  std::string result = "";
  std::array<char, 256> buffer;

  FILE *pipe = popen(cmd, "r");
  if (!pipe) {
    std::runtime_error("popen() failed.");
  }

  while (std::fgets(buffer.data(), buffer.size(), pipe) != NULL) {
    result += buffer.data();
  }

  int exit_status = pclose(pipe);
  if (exit_status != 0) {
    std::cerr << "Command failed with exit code: " << exit_status << std::endl;
  }

  return result;
}

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

    std::cout << "Savings songs from the directory: " << file << std::endl;

    for (const auto &entry : fs::directory_iterator(file)) {
      if (entry.is_regular_file(ec)) {
        std::cout << "\nFile in directory " << file << ": " << entry.path()
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
  * 2. Save metadata for Title, Artist and Album to a track data type
  * 3. get the Youtube ID of the audio file
  * process the file by creating the mel spectrogram operations then store the
  * spectrogram to the database
  * move the new wav file to the processed songs directory

  */
  // add single quote since input file paths can contain invalid characters for
  // command line arg
  std::string inputFilePath = "'" + filePath + "'";
  wav::MediaProbeInfo metadata = wav::GetMetadata(inputFilePath);

  for (const auto &stream : metadata.streams) {
    std::cout << "Stream Index: " << stream.index << std::endl;
    std::cout << "Codec Name: " << stream.codec_name << std::endl;
    std::cout << "Sample Rate: " << stream.sample_rate << std::endl;
    std::cout << "Tags: " << std::endl;
    for (const auto &pair : stream.tags) {
      std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "-----------" << std::endl;
  }

  std::cout << "Format Info" << std::endl;
  std::cout << "File Name: " << metadata.format.filename << std::endl;
  std::cout << "Duration: " << metadata.format.duration << std::endl;

  std::map<std::string, std::string> tags = metadata.format.tags;
  std::cout << "Artist: " << tags["artist"] << std::endl;
  std::cout << "Title: " << tags["title"] << std::endl;
  std::cout << "Album: " << tags["album"] << std::endl;

  std::cout << "--------END--------" << std::endl;
}

} // namespace utils
